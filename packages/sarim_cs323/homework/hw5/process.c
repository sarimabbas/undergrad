/* --------------------- IMPORTS  --------------------- */

#define _GNU_SOURCE
#include <libgen.h>  // dirname()
#include <limits.h>  // PATH_MAX
#include <signal.h>
#include <string.h>
#include <sys/stat.h>  // mkdir()
#include "process-stub.h"

#define UNINIT_INDEX -1
#define UNREADABLE_PIPE -1
#define TEMP_PATH_SIZE 20
#define INVALID_FILE_DESCRIPTOR -1
#define UNINIT_PID -5
#define DIGIT_STR_SIZE 256
#define PROCESS_TABLE_SIZE 100
#define UNINIT_STATUS -11

/* --------------------- GLOBALS --------------------- */

bool built_in_ignore;

/* --------------------- STRUCTS --------------------- */

typedef struct organize {
    // indexes for a stage
    int args_index,
        locals_index, 
        redirections_index,
        subcommands_index;

    // lengths of each portion
    int args_length,
        locals_length,
        redirections_length,
        subcommands_length;

    // total length of all tokens
    int total_length;

    // where did the parse stop? 
    int hanging_index;

    // optional error reporting
    int return_code; 

} organize;

typedef struct entry {
    int pid, status;
} entry;

typedef struct saved {
    int stdin, stdout, stderr;
} saved;

typedef struct outcome {
    int hanging_index;
    int return_code;
} outcome;

/* --------------------- DECLARATIONS --------------------- */

int outcome_init(outcome* oc);

outcome process_command(token* tok, int start_from);
outcome process_subcommand(token* tok, int start_from, saved* s);
outcome process_list(token* tok, int start_from);
outcome process_and_or(token* tok, int start_from);
outcome process_pipeline(token* tok, int start_from);
outcome process_stage(token* tok, organize* o, int* fdin, entry* table, int* table_index, int start_from);

outcome organize_and_or(token* tok, int start_from);
outcome organize_pipeline(token* tok, int start_from);
organize organize_stage(token* tok, int start_from);
organize organize_core(token* tok, int start_from);
bool is_delimiter(token* tok, int index);
int organize_init(organize* o);
int organize_debug(organize* o);

// helpers
int set_env_multi(token* tok, organize* o);
int unset_env_multi(token* tok, organize* o);

int set_redirects(token* tok, organize* o, int* here_doc_fd, int here_doc_count);
int unset_redirects(token* tok, organize* o, saved* s);
saved save(void);
int unsave(saved* s);
int organize_execvp(token* tok, organize* o, char** args);

// here docs
int count_here_docs(token* tok, organize* o);
int create_here_docs(token* tok, organize* o, int* here_doc_fd);
int clean_here_docs(int* here_doc_fd, int here_doc_count);


int create_dir(char* dirname);
int reap_zombies(char* mode);
int entry_table_init(entry* table, int len);
int set_status(int status);

// built in commands
bool built_in_commands_exist(token* tok, organize* o);
outcome built_in_commands(token* tok, organize* o);
int built_in_cd(token* tok, organize* o);
int built_in_wait(token* tok, organize* o);


/* --------------------- DEFINITIONS --------------------- */

int reap_zombies(char* mode) {
    int pid;
    int status;

    // don't hang
    if (strcmp(mode, "nohang") == 0) {
        // fprintf(stderr, "reaping no hanng\n");
        while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
            fprintf(stderr, "Completed: %d (%d)\n", pid, STATUS(status));
        }

    // hang until children finished
    } else {
        // fprintf(stderr, "reaping hang\n");
        while ((pid = waitpid(-1, &status, 0)) > 0) {
            fprintf(stderr, "Completed: %d (%d)\n", pid, STATUS(status));
        }
    }

    return EXIT_SUCCESS;
}

/**
 * @brief : process a parsnip token list
 */
int process(token* tok) {
    // hotfix for builtins
    built_in_ignore = false;
    // restore default signal handlinng
    signal(SIGINT, SIG_DFL);
    // reap zombies
    reap_zombies("nohang");
    // process command and set status
    outcome co = process_command(tok, 0);
    if (co.return_code == EXIT_SUCCESS) {
        set_status(0);
    }
    // return
    return co.return_code;
}

int outcome_init(outcome* oc) {
    oc->return_code = EXIT_SUCCESS;
    oc->hanging_index = UNINIT_INDEX;
    return EXIT_SUCCESS;
}

/* --------------------- Process functions --------------------- */

/**
 * @brief : attempt to parse a command
 * @param start_from : where to start parsing token list
 */
outcome process_command(token* tok, int start_from) {
    // prepare outcome
    outcome co;
    outcome_init(&co);
    // attempt to parse a list
    co = process_list(tok, start_from);
    // return outcome
    return co;
}

outcome process_subcommand(token* tok, int start_from, saved* s) {
    // init
    outcome sc;
    outcome_init(&sc);
    int pid;
    // error check
    if ((pid = fork()) < 0) {
        set_status(errno);
        perror("fork");
        exit(errno);
    }
    // child: exit with result
    if (pid == 0) {
        // unsave(s);
        sc = process_command(tok, start_from);
        exit(sc.return_code);
    // parent: listen to child process (waitpid is only for immediate children)
    } else {
        // ignore kills
        signal(SIGINT, SIG_IGN);
        // wait for subshell
        int status;
        pid = waitpid(pid, &status, 0); // TODO: wait for the subcommand explicitly?
        sc.return_code = STATUS(status);
        // listen for kills again
        signal(SIGINT, SIG_DFL);
        return sc;
    }
}

/**
 * @brief : attempt to parse a list
 */
outcome process_list(token* tok, int start_from) {
    // vars
    outcome oao;
    outcome pao;
    outcome_init(&oao);
    outcome_init(&pao);

    while(true) {

        // speculate first
        oao = organize_and_or(tok, start_from);

        // fork into background
        if (tok[oao.hanging_index].type == SEP_BG) {
            // attempt fork
            int pid;
            if ((pid = fork()) < 0) {
                set_status(errno);
                perror("fork");
                exit(errno);
            }
            // child process
            if (pid == 0) {
                pao = process_and_or(tok, start_from);
                exit(pao.return_code);
            // parent process
            } else {
                set_status(0);
                fprintf(stderr, "Backgrounded: %d\n", pid);
                pao.hanging_index = oao.hanging_index;
            }
        // proceed as normal
        } else  {
            pao = process_and_or(tok, start_from);
        }

        // are more and_ors forth coming?
        if (
            tok[oao.hanging_index].type != SEP_BG &&
            tok[oao.hanging_index].type != SEP_END
        ) {
            break;
        }

        // do over
        start_from = oao.hanging_index + 1;

        // no point continuing if you're just gonna hit another delimiter
        // *: it tries again to look for possibly more <and_or> ; <and_or>
        // *: SEP_END is automatically added by parsnip
        if (is_delimiter(tok, start_from)) {
            if(tok[start_from].type != PAR_LEFT) {
                pao.hanging_index = start_from;
                break;
            }
        }
    }

    // return outcome
    return pao;
}

/**
 * @brief : attempt to parse an and_or
 */
outcome process_and_or(token* tok, int start_from) {
    outcome ao;
    outcome_init(&ao);

    // while true:
    //      if the hanging index is AND
    //          if the return code was bad
    //                  skip following <pipeline> using the skip code
    //          if the return code was good
    //                  evaluate following <pipeline> using process_pipeline
    //      if the hanging index is OR
    //          if the return code was bad
    //                  evaluate the following <pipeline> using process_pipeline
    //          if the return code was good
    //                  skip the following pipeline using the skip code

    outcome po = process_pipeline(tok, start_from);
    while(true) {

        // if the hannging index is AND
        if (tok[po.hanging_index].type == SEP_AND) {
            if (po.return_code == EXIT_SUCCESS) {
                po = process_pipeline(tok, po.hanging_index + 1);
            } else {
                po = organize_pipeline(tok, po.hanging_index + 1);
            }
        } else if (tok[po.hanging_index].type == SEP_OR) {
            if (po.return_code == EXIT_SUCCESS) {
                po = organize_pipeline(tok, po.hanging_index + 1);
            } else {
                po = process_pipeline(tok, po.hanging_index + 1);
            }
        } else {
            ao.hanging_index = po.hanging_index;
            break;
        }
    }

    // return outcome
    ao.return_code = po.return_code;
    return ao;
}

/**
 * @brief : attempt to parse a pipeline
 */
outcome process_pipeline(token* tok, int start_from) {
    outcome po;
    outcome_init(&po);

    // vars
    int fdin = UNREADABLE_PIPE;
    int table_len = 100;
    entry table[table_len];
    int table_hanging_index = 0; // no of children in table
    entry_table_init(table, table_len);

    // greedy parse
    while(true) {

        // organize a stage
        organize o = organize_stage(tok, start_from);

        // don't pursue the stage if it has no arguments
        // *: it tries again to look for possibly more <and_or> ; <and_or>
        // *: SEP_END is automatically added by parsnip
        // if (o.args_length < 1) {
        //     po.hanging_index = o.hanging_index;
        //     break;
        // }

        // run the stage
        po = process_stage(tok, &o, &fdin, table, &table_hanging_index, start_from);

        // see if another stage is following
        if (tok[o.hanging_index].type != PIPE) {
            po.hanging_index = o.hanging_index;
            break;
        }

        // break if process failure
        if (po.return_code != EXIT_SUCCESS) {
            break;
        }

        // parse next stage
        start_from = o.hanging_index + 1;
    }

    // wait for children to die
    // if (table_hanging_index > 0) {
    //     int pid;
    //     int status;

    //     for (int i = 0;
    //          i < table_hanging_index;) {  // TODO: why does stuff break if you
    //                                       // channge this to table_len?

    //         // wait for any child process to die
    //         pid = waitpid(-1, &status, 0);

    //         // fprintf(stderr, "inside fork: %i\n", pid);

    //         // set pipeline status // TODO: do you need to change it to
    //         if (status != EXIT_SUCCESS) {
    //             set_status(STATUS(status));
    //             po.return_code = STATUS(status);
    //         }

    //         // get j to be the position in the table of the current pid
    //         int j = 0;
    //         while (j < table_len) {
    //             if (table[j].pid == pid) {
    //                 break;
    //             }

    //             j++;
    //         }

    //         // ignore zombie processes // TODO
    //         if (j < table_len) {
    //             table[j].status = status;
    //             i++;
    //         }
    //     }
    // }


    // wait for children to die
    signal(SIGINT, SIG_IGN);

    int status;
    for(int i = 0; i < table_hanging_index; i++) {
        waitpid(table[i].pid, &status, 0);
        if (status != EXIT_SUCCESS) {
            set_status(STATUS(status));
            po.return_code = STATUS(status);
        }
    }

    signal(SIGINT, SIG_DFL);

    // return outcome
    return po;
}

outcome process_stage(token* tok, organize* o, int* fdin, entry* table, int* table_index, int start_from) {
    // prepare outcome
    outcome so;
    outcome_init(&so);

    // vars that may or may not be needed
    int delimiter = tok[o->hanging_index].type;     // hanging delimiter
    int pipe_fd[2];                                 // for pipe
    int here_doc_count = count_here_docs(tok, o);   // for here docs
    int here_doc_fd[here_doc_count];
    // int start_from = o->hanging_index + 1;          // continue after delimiter
    int pid = UNINIT_PID;                           // for fork
    char* args[o->args_length + 1];                 // for execvp
    saved s = save();                               // all the std file descriptors
    int par_left_pos = -1;                          // if subcmd

    // at this point the organize function has broken at a delimiter
    //  create here docs
    //  set env
    //  set redirects
    //  if delimiter == (
    //      so.hanging_index = speculate position of right paren
    //      so.return_code = process_subcommand(toks[left paren + 1:], here_docs)
    //                          => fork
    //  if delimiter == |
    //      create pipe
    //  if built in commands
    //      set pipe (if any)
    //      so.return code = built in commands
    //      unset pipe
    //  else    
    //      fork
    //          child
    //              deal with pipe
    //              launch execvp (hopefully args exist)
    //          parent
    //              save to table
    //              deal with pipe
    //  unset env
    //  unset redirects
    //  clean here docs

    // attempt recovery in case of failure instead of exiting
    if (create_here_docs(tok, o, here_doc_fd) != EXIT_SUCCESS) {
        unsave(&s);
        so.return_code = EXIT_FAILURE;
        return so;
    }

    if (set_env_multi(tok, o) != EXIT_SUCCESS) {
        clean_here_docs(here_doc_fd, here_doc_count);
        unsave(&s);
        so.return_code = EXIT_FAILURE;
        return so;
    }

    if (set_redirects(tok, o, here_doc_fd, here_doc_count) != EXIT_SUCCESS) {
        clean_here_docs(here_doc_fd, here_doc_count);
        unset_env_multi(tok, o);
        unsave(&s);
        so.return_code = EXIT_FAILURE;
        return so;
    }

    if (delimiter == PIPE) {
        if (pipe(pipe_fd) < 0) {
            set_status(errno);
            perror("organize_stage:pipe failure\n");
            exit(errno);
        }
    }

    // run subcommand if any
    // for (int i = start_from; i < o->hanging_index; i++) { if (tok[i].type == PAR_LEFT) { par_left_pos = i; } }
    // if (par_left_pos != -1) {
    //     // hopefully the correct so.index is updated here
    //     so = process_subcommand(tok, par_left_pos + 1, &s); 

    //     // most likely the so.hanging index won't be a right paren so we'll have to increment
    //     // TODO: don't really need this because the process_pipeline() already knows end of stage via organize
    //     if (tok[so.hanging_index + 1].type == PAR_RIGHT) {
    //         so.hanging_index = so.hanging_index + 1;
    //     }

    // // if built in command
    // } else 
    if (built_in_commands_exist(tok, o)) {

        // deal with pipe e.g. cd -p | wc
        // set pipe
        if (delimiter == PIPE) {
            built_in_ignore = true;
            *fdin = pipe_fd[0];  // update fdin for next stage
            if (pipe_fd[1] != STDOUT_FILENO) {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
        }
        // run commands
        so =  built_in_commands(tok, o);
        // unset pipe
        dup2(s.stdout, STDOUT_FILENO);

    // regular commands
    } else {
        
        // attempt fork
        if ((pid = fork()) < 0) {
            set_status(errno);
            perror("fork");
            exit(errno);
        }

        // child process
        if (pid == 0) {

            // close saved parent fd 
            unsave(&s);

            // stdin = read[last pipe]
            if (*fdin != UNREADABLE_PIPE) {
                dup2(*fdin, STDIN_FILENO);
                close(*fdin);
            }

            // if pipe was created
            if (tok[o->hanging_index].type == PIPE) {
                // no reading from new pipe
                close(pipe_fd[0]);

                // stdout = write[new pipe]
                if (pipe_fd[1] != STDOUT_FILENO) {
                    dup2(pipe_fd[1], STDOUT_FILENO);
                    close(pipe_fd[1]);
                }
            }

            // execute any subcommands
            for (int i = start_from; i < o->hanging_index; i++) { if (tok[i].type == PAR_LEFT) { par_left_pos = i; } }
            if (par_left_pos != -1) {
                so = process_subcommand(tok, par_left_pos + 1, &s);
                exit(so.return_code);
            }

            // prepare args for execvp
            organize_execvp(tok, o, args);

            // run program
            if (execvp(tok[o->args_index].text, args) < 0) {
                set_status(errno);
                fprintf(stderr, "execvp(%s): ", tok[o->args_index].text);
                perror("");
                exit(errno);
            }
            perror("organize_stage:reached after execvp()\n");
            exit(EXIT_FAILURE);

        // parent process
        } else {

            // record the child in table
            table[*table_index].pid = pid;
            (*table_index)++;

            // close read[last pipe]
            if (*fdin != UNREADABLE_PIPE) {
                close(*fdin);
            }

            // if pipe was created by bashful
            if (tok[o->hanging_index].type == PIPE) {
                *fdin = pipe_fd[0];  // update fdin for next stage
                close(pipe_fd[1]);   // no writing to new pipe
            }
        }
    }

    // signal(SIGINT, SIG_IGN);  // Ignore interrupts

    // unset
    unset_redirects(tok, o, &s);
    unset_env_multi(tok, o);
    clean_here_docs(here_doc_fd, here_doc_count);
    unsave(&s);

    // return stage outcome
    return so;
}











/* --------------------- Organize functions --------------------- */

outcome organize_and_or(token* tok, int start_from) {
    outcome ao;
    outcome_init(&ao);

    while(true) {
        outcome po = organize_pipeline(tok, start_from);
        if (
            tok[po.hanging_index].type != SEP_AND &&
            tok[po.hanging_index].type != SEP_OR
        ) {
            ao.hanging_index = po.hanging_index;
            break;
        }
        start_from = po.hanging_index + 1;
    }

    return ao;
}

outcome organize_pipeline(token* tok, int start_from) {
    outcome po;
    outcome_init(&po);

    while (true) {
        organize o = organize_stage(tok, start_from);
        if (tok[o.hanging_index].type != PIPE) {
            po.hanging_index = o.hanging_index;
            break;
        }
        start_from = o.hanging_index + 1;
    }

    return po;
}

/**
 * @brief : attempt to organize the tokens
 */
organize organize_stage(token* tok, int start_from) {
    int left_paren_count = 0;
    int right_paren_count = 0;

    // collect all tokens up to a delimiter
    organize o = organize_core(tok, start_from);
    if (tok[o.hanging_index].type == PAR_LEFT) {
        left_paren_count++;
    }

    int stage_hanging_index = o.hanging_index;
    while(left_paren_count != right_paren_count) {
        
        // parse again until you hit a delimiter
        organize another = organize_core(tok, stage_hanging_index + 1);
        stage_hanging_index = another.hanging_index;

        // make sure the paren counts are equal
        if (tok[another.hanging_index].type == PAR_LEFT) {
            left_paren_count++;
        } else if (tok[another.hanging_index].type == PAR_RIGHT) {
            right_paren_count++;
        }
    }

    // update the true hanging index
    if (tok[stage_hanging_index].type == PAR_RIGHT) {
        o.hanging_index = stage_hanging_index + 1;
    } else {
        o.hanging_index = stage_hanging_index;
    }

    return o;
}

bool is_delimiter(token* tok, int index) {
    int t = tok[index].type;
    if (
        t == NONE ||
        t == PIPE ||        // |

        t == SEP_AND ||     // &&
        t == SEP_OR ||      // ||

        t == SEP_END ||     // ;
        t == SEP_BG ||      // &

        t == PAR_LEFT ||    // (
        t == PAR_RIGHT      // )
        ) {
            return true;
    } else {
        return false;
    }
}

organize organize_core(token* tok, int start_from) {
    // prepare outcome
    organize o;
    organize_init(&o);

    // set starting point for parse
    int index = start_from;
    while (true) {

        // break if reached a delimiter
        if (is_delimiter(tok, index)) {
            break;
        }

        if (tok[index].type == ARG) {
            // first time
            if(o.args_index == UNINIT_INDEX) {
                o.args_index = index;
            }
            // increment count
            (o.args_length)++;
        }

        if (tok[index].type == LOCAL) {
            // first time
            if(o.locals_index == UNINIT_INDEX) {
                o.locals_index = index;
            }
            // increment count
            (o.locals_length)++;
        }

        if (RED_OP(tok[index].type)) {
            // first time
            if(o.redirections_index == UNINIT_INDEX) {
                o.redirections_index = index;
            }
            // increment count
            (o.redirections_length)++;
        }

        if (tok[index].type == SUBCMD) {
            // first time
            if(o.subcommands_index == UNINIT_INDEX) {
                o.subcommands_index = index;
            }
            // increment count
            (o.subcommands_length)++;
        }

        // move parser forward
        index++;
    }

    // set total length
    o.total_length = \
        o.args_length + \
        o.locals_length + \
        o.redirections_length + \
        o.subcommands_length;

    // keep track of the index that the loop ended at
    o.hanging_index = index;

    // return
    o.return_code = EXIT_SUCCESS;
    return o;
}


/* --------------------- Helper functions --------------------- */

saved save(void) {
    saved s;
    s.stdin = dup(STDIN_FILENO);
    s.stdout = dup(STDOUT_FILENO);
    s.stderr = dup(STDERR_FILENO);
    return s;
}
 
int unsave(saved* s) {
    close(s->stdin);
    close(s->stdout);
    close(s->stderr);
    return EXIT_SUCCESS;
}

int organize_execvp(token* tok, organize* o, char** args) {
    int start = o->args_index;
    int end = o->args_index + o->args_length;
    for (int i = start, j = 0; i < end; i++, j++) {
        args[j] = tok[i].text;
    }
    args[o->args_length] = NULL;
    return EXIT_SUCCESS;
}


/**
 * @brief : helper for organize_stage
 */
int organize_init(organize* o) {
    // index for a stage
    o->args_index = \
    o->locals_index = \
    o->redirections_index = \
    o->subcommands_index = UNINIT_INDEX;

    // lengths of each portion
    o->args_length = \
    o->locals_length = \
    o->redirections_length = \
    o->subcommands_length = \

    // total length of all tokens
    o->total_length = 0;

    // where did the parse stop?
    o->hanging_index = 0;

    // optional error reporting
    o->return_code = EXIT_SUCCESS;

    // finished
    return EXIT_SUCCESS;
}

/**
 * @brief : helper for organize_stage
 */
int organize_debug(organize* o) {
    printf("INDEXES:\nARGS: %i, LOCALS: %i, REDIR: %i, SUBCMDS: %i\n\
            LENGTHS:\nARGS: %i, LOCALS: %i, REDIR: %i, SUBCMDS: %i, TOTAL:%i\n", 
            o->args_index, o->locals_index, o->redirections_index, o->subcommands_index, 
            o->args_length, o->locals_length, o->redirections_length, o->subcommands_length, 
            o->total_length); 
    return EXIT_SUCCESS;
}

int set_env_multi(token* tok, organize* o) {

    if (o->locals_length < 1) {
        return EXIT_SUCCESS;
    }

    // environmental variables are set from local tokens
    int start = o->locals_index;
    int end = o->locals_index + o->locals_length;

    for (int i = start; i < end; i++) {

        // duplicate token text
        int len = strlen(tok[i].text);
        char dup[len + 1];
        for (int i = 0; i < len + 1; i++) {
            dup[i] = '\0';
        }
        strcpy(dup, tok[i].text);

        // split by '=' sign
        char* var = strtok(dup, "=");
        char* val = strtok(NULL, "=");

        // set to overwrite
        int res = setenv(var, val, 1);
        if (res == -1) {
            perror("setenv");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int unset_env_multi(token* tok, organize* o){
    if (o->locals_length < 1) {
        return EXIT_SUCCESS;
    }

    // environmental variables are set from local tokens
    int start = o->locals_index;
    int end = o->locals_index + o->locals_length;

    // 
    for (int i = start; i < end; i++) {
        // duplicate token text
        int len = strlen(tok[i].text);
        char dup[len + 1];
        for (int i = 0; i < len + 1; i++) {
            dup[i] = '\0';
        }
        strcpy(dup, tok[i].text);

        // split by '=' sign
        char* var = strtok(dup, "=");

        // unset
        int res;
        if ((res = unsetenv(var)) < 0) {
            set_status(errno);
            perror("setenv");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int set_redirects(token* tok, organize* o, int* here_doc_fd, int here_doc_count) {

    if (o->redirections_length < 1) {
        return EXIT_SUCCESS;
    }

    // redirects are set from redirect tokens
    int start = o->redirections_index;
    int end = o->redirections_length + o->redirections_index;

    for(int i = start; i < end; i++) {

        switch(tok[i].type) {


            case RED_IN:
                ; // empty statement C quirk

                int red_in;
                if ((red_in = open(tok[i].text,
                        O_RDONLY    // read only
                    )) < 0) {

                    set_status(errno);
                    perror("RED_IN open error");
                    return EXIT_FAILURE;
                }

                // switch (dup2 errors may be ignored)
                dup2(red_in, STDIN_FILENO);
                close(red_in); // TODO: should I do this? We know that the old STDIN is closed,
                // TODO: but red_in may not be

                break;

            // * BATTLE TESTED
            case RED_OUT:
                ; // empty statement C quirk

                // create directory if needed
                if (create_dir(tok[i].text) != EXIT_SUCCESS) {
                    return EXIT_FAILURE;
                }

                // check that the filename isn't stupid
                char red_out_path_copy[PATH_MAX];
                strcpy(red_out_path_copy, tok[i].text);
                char* red_out_base = basename(red_out_path_copy);
                if (strcmp(red_out_base, ".") == 0 || strcmp(red_out_base, "/") == 0) {
                    // TODO: need to clean up created dir here?
                    break;
                }

                // attempt open
                int red_out;

                if ((red_out = open(tok[i].text,
                        O_WRONLY    // write mode
                        | O_CREAT   // create if needed
                        | O_TRUNC   // overwrite if exists
                        , 0777      // set permissions
                    )) < 0) {

                    set_status(errno);
                    perror("RED_OUT open error");
                    return EXIT_FAILURE;
                }

                // switch
                dup2(red_out, STDOUT_FILENO);
                close(red_out);

                break;

            // * SOMEWHAT BATTLE TESTED
            case RED_OUT_APP:
                ; // empty statement C quirk

                // create directory if needed
                create_dir(tok[i].text);

                // check that the filename isn't stupid
                char red_app_path_copy[PATH_MAX];
                strcpy(red_app_path_copy, tok[i].text);
                char* red_app_base = basename(red_app_path_copy);
                if (strcmp(red_app_base, ".") == 0 || strcmp(red_app_base, "/") == 0) {
                    // TODO: need to clean up created dir here?
                    break;
                }

                int red_out_app;
                // open file in write mode, with append flag. create if needed
                if ((red_out_app = open(tok[i].text,
                        O_WRONLY    // write mode
                        | O_APPEND  // append if exists
                        | O_CREAT   // create if needed
                        , 0777      // set permissions
                    )) < 0) {

                    set_status(errno);
                    perror("RED_OUT_APP open error");
                    return EXIT_FAILURE;
                }

                // switch
                dup2(red_out_app, STDOUT_FILENO);
                close(red_out_app);

                break;

            case RED_ERR_OUT:
                // create a copy of STDOUT, and copy into STDERR
                dup2(STDOUT_FILENO, STDERR_FILENO);
                // TODO: DO I NEED close() here?
                break;

            // * DONE
            case RED_ERR_CLS:
                // close STDERR (ignore errors)
                close(STDERR_FILENO);
                // TODO: DO I NEED close() here?

                break;

            case RED_IN_HERE:
                break;

            case NONE:
                perror("set_redirects:NONE");
                return EXIT_FAILURE;
                break;

            default:
                perror("set_redirects:default");
                return EXIT_FAILURE;
                break;
        }
    }

    // leave here docs for last
    // ! Probably a problem if here_doc_count > 1
    // ! but assume will never happen b/c of parsnip
    for (int i = 0; i < here_doc_count; i++) {

        // switch file descriptors
        dup2(here_doc_fd[i], STDIN_FILENO);
        close(here_doc_fd[i]);
    }

    return EXIT_SUCCESS;
}

int unset_redirects(token* tok, organize* o, saved* s) {
    
    if (o->redirections_length < 1) {
        return EXIT_SUCCESS;
    }

    // redirects are set from redirect tokens
    int start = o->redirections_index;
    int end = o->redirections_length + o->redirections_index;

    for (int i = start; i < end; i++) {

        switch (tok[i].type) {

            case RED_OUT: case RED_OUT_APP:

                dup2(s->stdout, STDOUT_FILENO);
                break;

            case RED_IN: case RED_IN_HERE:
                dup2(s->stdin, STDIN_FILENO);
                break;
            
            case RED_ERR_OUT: case RED_ERR_CLS:
                dup2(s->stderr, STDERR_FILENO);
                break;

            default:
                break;

        }
    }

    return EXIT_SUCCESS;
}

int count_here_docs(token* tok, organize* o) {
    int here_doc_count = 0;
    int start = o->redirections_index;
    int end = o->redirections_index + o->redirections_length;
    for (int i = start; i < end; i++) {
        if (tok[i].type == RED_IN_HERE) {
            here_doc_count++;
        }
    }
    return here_doc_count;
}

int clean_here_docs(int* here_doc_fd, int here_doc_count) {
    if (here_doc_count < 1)  {
        return EXIT_SUCCESS;
    }

    // close all file descriptors
    for (int i = 0; i < here_doc_count; i++) {
        close(here_doc_fd[i]);
    }

    return EXIT_SUCCESS;
}

int create_here_docs(token* tok, organize* o, int* here_doc_fd) {

    if (o->redirections_length < 1) {
        return EXIT_SUCCESS;
    }

    int here_doc_count = 0;

    int start = o->redirections_index;
    int end = o->redirections_index + o->redirections_length;

    for (int i = start; i < end; i++) {

        if (tok[i].type == RED_IN_HERE) {

            // create here doc
            char template[] = "tmpfile.XXXXXX";
            int here_fd;
            if ((here_fd = mkstemp(template)) < 0) {
                set_status(errno);
                perror("RED_IN_HERE mkstemp error");
                return EXIT_FAILURE;
            }

            // unlink file so that we don't have to worry about it later
            unlink(template);

            // copy content over
            if (write(here_fd, tok[i].text, strlen(tok[i].text)) < 0) {
                set_status(errno);
                perror("RED_IN_HERE write error");
                return EXIT_FAILURE;
            }

            // rewind to the start so we can read later
            lseek(here_fd, 0, SEEK_SET);

            // safekeeping the name and file descriptor
            here_doc_fd[here_doc_count] = here_fd;

            // increment for next
            here_doc_count++;
        }
    }

    return EXIT_SUCCESS;
}

int create_dir(char* tok_text) {
    // make a copy of the string
    char path_copy[PATH_MAX];
    strcpy(path_copy, tok_text);

    // get the directory path
    char* dir = dirname(path_copy);

    // no need to create if no proper directory
    if (strcmp(dir, ".") == 0) {
        return EXIT_SUCCESS;
    }

    // create directory
    if (mkdir(dir, 0777) < 0) {

        // ignore if directory exist alreadz
        if (errno == EEXIST) {
           return EXIT_SUCCESS;
        }

        set_status(errno);
        perror("RED_OUT mkdir error");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int entry_table_init(entry* table, int len) {
    for (int i = 0; i < len; i++) {
        table[i].pid = UNINIT_PID;
        table[i].status = UNINIT_STATUS;
    }
    return EXIT_SUCCESS;
}

int set_status(int status) {
    char str[DIGIT_STR_SIZE];
    for(int i = 0; i < DIGIT_STR_SIZE; i++) {
        str[i] = '\0';
    }
    sprintf(str, "%d", status);
    if (setenv("?", str, 1) < 0) {
        perror("");
        exit(errno);
    }
    return status;
}

/* --------------------- Built-in command functions --------------------- */

bool built_in_commands_exist(token* tok, organize* o) {
    if (o->args_length < 1) {
        return false;
    }

    // TODO: patch needed? within subcommand? start_from?
    int start_arg = o->args_index;
    if (strcmp(tok[start_arg].text, "cd") == 0 ||
        strcmp(tok[start_arg].text, "wait") == 0) {
        return true;
    }

    return false;
}

outcome built_in_commands(token* tok, organize* o) {

    // prepare outcome
    outcome bo;
    outcome_init(&bo);

    // don't attempt if no args
    if (o->args_length < 1) {
        return bo;
    }

    int start = o->args_index;

    // built-in cd
    if (strcmp(tok[start].text, "cd") == 0) {

        // failed for some reason
        if (built_in_cd(tok, o) != EXIT_SUCCESS) {

            // failure: fail and used
            bo.return_code = EXIT_FAILURE;
        }

    // built-in wait
    } else if (strcmp(tok[start].text, "wait") == 0) {

        // failed for some reason
        if (built_in_wait(tok, o) != EXIT_SUCCESS) {

            // failure: fail and used
            bo.return_code = EXIT_FAILURE;
        }
    } 

    // return final outcome
    return bo;
}

int built_in_cd(token* tok, organize* o) {

    if (o->args_length < 1 || o->args_length > 2) {
        fprintf(stderr, "usage: cd [-p | dirName]\n"); // TODO: how should i print these error messages out?
        return EXIT_FAILURE;
    }

    char* path;

    // if cd and something else is specified 
    if (o->args_length == 2) {

        // print directory
        if (strcmp(tok[1].text, "-p") == 0) {

            // get current working directory
            char curr_path[PATH_MAX];
            if (getcwd(curr_path, PATH_MAX) == NULL) {
                perror("built_in_cd getcwd error");
                return EXIT_FAILURE;
            }

            // print path for user
            fprintf(stdout, "%s\n", curr_path);
            fflush(stdout);
            return EXIT_SUCCESS;

        // change directory
        } else {

            // see below for the changing code
            path = tok[1].text;
        }

    // if only cd is specified
    } else {

        // get path to HOME
        path = getenv("HOME");
    }

    // change directory into path
    if (!built_in_ignore) {
        if (chdir(path) < 0) {
            set_status(errno);
            perror("built_in_cd chdir error");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

int built_in_wait(token* tok, organize* o) {

    // ignore attempts to kill
    signal(SIGINT, SIG_IGN);

    if (o->args_length < 1 || o->args_length > 1) {
        fprintf(stderr, "usage: wait\n");
        return EXIT_FAILURE;
    }

    reap_zombies("hang");

    // resume signal handling
    signal(SIGINT, SIG_DFL);

    return EXIT_SUCCESS;
}

