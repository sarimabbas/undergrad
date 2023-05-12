#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wordexp.h>
#include <ctype.h>
#include "parsnip.h"
#include "parse.h"

// * GLOBALS
int LIST_SIZE = 10;

// redirect bools
int output_redirects;
int input_redirects;
int stderr_redirects;
bool output_redirects_flag;
bool input_redirects_flag;
bool stderr_redirects_flag;

// other error bools
bool multiple_commands;
bool unbalanced_paren;
bool null_command;
bool missing_filename;

token* parse(token *tok) { 
    // assign all globals
    output_redirects = 0;
    input_redirects = 0;
    stderr_redirects = 0;

    output_redirects_flag = false;
    input_redirects_flag = false;
    stderr_redirects_flag = false;

    multiple_commands = false;
    unbalanced_paren = false;
    null_command = false;
    missing_filename = false;

    // create list suited to the size of the token array
    int size = 0;
    for (int i = 0; tok[i].type != NONE; i++) {
        size++;
    }
    LIST_SIZE = 3 * size;
    list* l = malloc(sizeof(list));
    lSetup(l);

    // create position to track through the token array
    int pos = 0;


    // error checking
    bool overall_error = false;
    bool parse_result = synCommand(l, tok, &pos);

    // check that nothing follows
    if (tok[pos].type != NONE) {
        overall_error = true;
        multiple_commands = true;
    }

    if (!parse_result) {
        overall_error = true;
        fprintf(stderr, "parsnip: null command\n");
    } else if (multiple_commands) {
        overall_error = true;
        fprintf(stderr, "parsnip: multiple commands\n");
    } else if (output_redirects_flag) {
        overall_error = true;
        fprintf(stderr, "parsnip: multiple output redirects\n");
    } else if (input_redirects_flag) {
        overall_error = true;
        fprintf(stderr, "parsnip: multiple input redirects\n");
    } else if (stderr_redirects_flag) {
        overall_error = true;
        fprintf(stderr, "parsnip: multiple stderr redirects\n");
    } else if (unbalanced_paren) {
        overall_error = true;
        fprintf(stderr, "parsnip: unbalanced parenthesis\n");
    } else if (missing_filename) {
        overall_error = true;
        fprintf(stderr, "parsnip: missing filename\n");
    }


    if(overall_error) {
        lDestroy(l);
        return NULL;
    }

    // post-processing
    for(int i = 0; i < l->count; i++) {
        if (l->tokens[i].type == RED_IN_HERE) {
            // create an empty string
            char* builder = strdup("");

            // read in one line at a time
            char *line = NULL;
            size_t linecap = 0;
            ssize_t linelen;
            while ((linelen = getline(&line, &linecap, stdin)) > 0) {
                bool splice = false;
                bool end_whitespace = false;

                // strip the newline character, if any
                if (line[linelen - 1] == '\n') {
                    line[linelen - 1] = '\0';

                    if (line[linelen - 2] == '\\') {
                        line[linelen - 2] = '\0';
                        splice = true;
                    }

                    if(isspace(line[linelen -3])) {
                        end_whitespace = true;
                    }
                }

                // detect end of here document
                if(strcmp(line, l->tokens[i].text) == 0) {
                    // free line buffer used by readline/getline
                    free(line);
                    line = NULL;
                    // break out of loop
                    break;
                } else {
                    // scan for expansions
                    wordexp_t p;
                    char** w; // array of words/strings
                    wordexp(line, &p, 0);
                    w = p.we_wordv;
                    for (int k = 0; k < p.we_wordc; k++) {
                        // add each word to builder
                        char* word_cpy = strdup(w[k]);
                        builder = concat(builder, word_cpy, "free");

                        // add whitespace for all words if flag set
                        if (end_whitespace) {
                            char* whitespace = strdup(" ");
                            builder = concat(builder, whitespace, "free");
                        // else set whitespace for all but the last word
                        } else {
                            if (k != p.we_wordc - 1) {
                                char* whitespace = strdup(" ");
                                builder = concat(builder, whitespace, "free");
                            }
                        }
                    }
                    wordfree(&p);  // free all w[k]

                    // add a new line
                    if (!splice) {
                        char* newline = strdup("\n");
                        builder = concat(builder, newline, "free");
                    }

                    // free line
                    free(line);
                    line = NULL;
                }
            }

            // add builder to token
            free(l->tokens[i].text);
            l->tokens[i].text = builder;
        }
    }

    // return
    token* parsed = l->tokens;
    free(l);
    return parsed;
}

// * SYNTAX FUNCTIONS

bool synArgument(list* l, token* arr, int* pos) {
    // check if none
    if(arr[*pos].type == NONE) {
        return false;
    }

    // check if text
    if (!arr[*pos].text) {
        return false;
    }

    // check token as SIMPLE
    bool check = true;
    for (int i = 0, len = strlen(arr[*pos].text); i < len; i++) {
        if (!isSimple(arr[*pos].text[i])) {
            check = false;
        }
    }

    // add token to list if SIMPLE
    if (check) {
        lPush(l, &(arr[*pos]));
        token* added = lGet(l, -1);
        added->type = ARG;

        // advance position
        (*pos)++;
    }

    // return
    return check;
}

bool synFilename(list* l, token* arr, int* pos) {
    // check if none
    if (arr[*pos].type == NONE) {
        return false;
    }

    // check if text
    if (!arr[*pos].text) {
        return false;
    }

    // check token as SIMPLE
    bool check = true;
    for (int i = 0, len = strlen(arr[*pos].text); i < len; i++) {
        if (!isSimple(arr[*pos].text[i])) {
            check = false;
        }
    }

    // don't push token to array, because we want to add a single token
    // don't advance position
    // of the form <red_op><filename>
    return check;
}

// <local> = VARIABLE=VALUE
bool synLocal(list* l, token* arr, int* pos) {
    // check if none
    if (arr[*pos].type == NONE) {
        return false;
    }

    if (!arr[*pos].text) {
        return false;
    }

    // check syntax
    int len = strlen(arr[*pos].text);

    // LOCAL requires 2 chars e.g. A= minimum
    if (len < 2) { 
        return false;
    }

    char prev;
    char curr;
    int equals_count = 0;
    for(int i = 1; i < len; i++) {
        prev = arr[*pos].text[i - 1];
        curr = arr[*pos].text[i];
        // cannot begin with a digit
        if (isdigit(prev) && i == 1) {
            return false;
        } 
        // if the first character is =, then it is not valid
        if (i == 1 && prev == '=') { 
            return false; 
        }
        // if an unescaped equals is seen, it better be the only one
        if (curr == '=' && prev != '\\') {

            // quickly check if all the NAME chars are alphanumeric till this point
            for(int j = 0; j < i; j++) {
                if(!charInVarChr(arr[*pos].text[j])) {
                    return false;
                }
            }

            equals_count++;
        }
        // implicitly ignore other equals signs
    }

    // should've only observed one equals sign for VARIABLE=VALUE to be valid
    if (equals_count == 1) {
        lPush(l, &(arr[*pos]));
        token* just_added = lGet(l, -1);
        just_added->type = LOCAL;

        // advance position
        (*pos)++;

        return true;
    } else {
        return false;
    }
}

bool synRedOp(list* l, token* arr, int* pos) {
    // check if none
    if (arr[*pos].type == NONE) {
        return false;
    }

    // check syntax
    if (
        arr[*pos].type == RED_IN ||
        arr[*pos].type == RED_OUT ||
        arr[*pos].type == RED_IN_HERE ||
        arr[*pos].type == RED_OUT_APP
    ) {
        // don't push token to array, because we want to add a single token
        // of the form <red_op><filename>
        return true;
    } else {
        return false;
    }
}

bool synRedSym(list* l, token* arr, int* pos) {
    // check if none
    if (arr[*pos].type == NONE) {
        return false;
    }

    if (arr[*pos].type == RED_ERR_OUT || 
        arr[*pos].type == RED_ERR_CLS) {
        lPush(l, &(arr[*pos]));
        token* just_added = lGet(l, -1);
        just_added->type = arr[*pos].type;

        // advance position
        (*pos)++;

        return true;
    } else {
        return false;
    }
}

// <redirect> = <red_op><filename> / <red_sym>
bool synRedirect(list* l, token* arr, int* pos) {
    // check if none
    // if (arr[*pos].type == NONE) {
    //     return false;
    // }

    int one_ahead = *pos + 1;
    if (synRedOp(l, arr, pos)) {
        if (synFilename(l, arr, &one_ahead)) {
            // if both were successful, construct a token
            token* combined = malloc(sizeof(token));
            // set the type
            combined->type = arr[*pos].type;
            // set the string
            combined->text = strdup(arr[*pos + 1].text);
            // push to list
            lPush(l, combined);
            // free original
            freeToken(combined);
            // advance position by 2
            *pos += 2; 
            // return true because <red_op><filename> works
            return true;
        } else {
            // return false because <red_op> but no filename
            missing_filename = true;
            return false;
        }
    } else if (synRedSym(l, arr, pos)) {
        // synRedSym will add by itself
        return true;
    } else {
        return false;
    }
}

// <prefix> = [ <local> / redirect ]*
// 0 or more occurences of <local> / <redirect>
// I guess it can always return true
bool synPrefix(list* l, token* arr, int* pos) {
    // check if none
    if (arr[*pos].type == NONE) {
        return true; 
    }

    // 0 or more occurences of below
    while(
        synLocal(l, arr, pos) || 
        synRedirect(l, arr, pos)
    );

    // kleene star so always return true
    return true;
}

bool synSuffix(list* l, token* arr, int* pos) {
    // check if none
    if (arr[*pos].type == NONE) {
        return true; 
    }

    // 0 or more occurences of below
    while (
        synArgument(l, arr, pos) || 
        synRedirect(l, arr, pos)
    );

    // kleene star so always return true
    return true;
}

bool synRedList(list* l, token* arr, int* pos) {
    // check if none
    if (arr[*pos].type == NONE) {
        return true; 
    }

    while(synRedirect(l, arr, pos));

    return true;
}

bool synSimple(list* l, token* arr, int* pos) {
    // check if none
    if (arr[*pos].type == NONE) {
        return false;
    }
    
    int backup_pos = *pos;
    int backup_count = l->count;

    if (synPrefix(l, arr, pos) && 
        synArgument(l, arr, pos) && 
        synSuffix(l, arr, pos)) {

            return true;
    } else {
        // needs to be a way to reset cos you already added simple
        // just pop the count difference
        int current_count = l->count;
        for(int i = 0; i < current_count - backup_count; i++) {
            lDeleteLast(l);
        }

        // restore position
        *pos = backup_pos;

        return false;
    }
}

bool synSubCmd(list* l, token* arr, int* pos) {
    // check if none
    if (arr[*pos].type == NONE) {
        return false;
    }

    // first try prefix
    if (!synPrefix(l, arr, pos)) {
        return false;
    }

    // then check for left paren
    if(arr[*pos].type != PAR_LEFT) {
        return false;
    } else {
        // push left parent onto token array
        lPush(l, &(arr[*pos]));
        token* just_added = lGet(l, -1);
        just_added->type = PAR_LEFT;

        // advance position
        (*pos)++;
    }

    // then check for command
    if (!synCommand(l, arr, pos)) {
        return false;
    }

    // then check for right paren
    if (arr[*pos].type != PAR_RIGHT) {
        // set error flag to true
        unbalanced_paren = true;
        return false;
    } else {
        // right parent successful
        lPush(l, &(arr[*pos]));
        token* just_added = lGet(l, -1);
        just_added->type = PAR_RIGHT;

        // advance position
        (*pos)++;
    }

    // then check for redirect list
    if (!synRedList(l, arr, pos)) {
        return false;
    }

    // return true if all checks passed
    return true;
}

bool synStage(list* l, token* arr, int* pos) {
    int start_of_stage = l->count;

    // check if none
    if (arr[*pos].type == NONE) {
        return false;
    }

    if (synSimple(l, arr, pos) 
        || synSubCmd(l, arr, pos)) {

        // check for multiple redirects
        for(int i = start_of_stage; i < l->count; i++) {
            
            // check up until the end of the stage, except
            // if you hit a token you're not supposed to enter
            if (l->tokens[i].type == SEP_END 
                || l->tokens[i].type == SEP_BG 
                || l->tokens[i].type == PAR_RIGHT
                || l->tokens[i].type == PAR_LEFT) {
                    break;
            }

            if (l->tokens[i].type == RED_IN || l->tokens[i].type == RED_IN_HERE) {
                input_redirects++;
            } else if (l->tokens[i].type == RED_OUT || l->tokens[i].type == RED_OUT_APP) {
                output_redirects++;
            } else if (l->tokens[i].type == RED_ERR_CLS || l->tokens[i].type == RED_ERR_OUT) {
                stderr_redirects++;
            }
        } 
 
        // set flags permanently
        if (input_redirects > 1) {
            input_redirects_flag = true;
        }
        if (output_redirects > 1) {
            output_redirects_flag = true;
        }
        if (stderr_redirects > 1) {
            stderr_redirects_flag = true;
        }

        // reorder the stage
        lStageReorder(l, start_of_stage);

        return true;
    } else {
        return false;
    }
}

bool synPipeline(list* l, token* arr, int* pos) {
    // reset input/output counts for next stage
    input_redirects = 0;
    output_redirects = 0;
    stderr_redirects = 0;

    // check if none
    if (arr[*pos].type == NONE) {
        return false;
    }

    // at least one stage must be parsed
    if(!synStage(l, arr, pos)) {
        return false;
    }

    // now any extra stages may be parsed
    while(true) {

        // check for PIPE
        if (arr[*pos].type != PIPE) {
            // break if no pipe
            break;
        } else {
            // push token onto array
            lPush(l, &(arr[*pos]));
            token* just_added = lGet(l, -1);
            just_added->type = PIPE;

            // advance position
            (*pos)++;
        }

        // increment output redirect counts for previous stage
        output_redirects++;
        if (output_redirects > 1) {
            output_redirects_flag = true;
        }

        // reset counts for next stage
        input_redirects = 1;
        stderr_redirects = 0;
        output_redirects = 0;

        // expect a stage to follow the pipe always
        if(!synStage(l, arr, pos)) {
            return false;
        }
    }

    // reset counts 
    input_redirects = 0;
    output_redirects = 0;
    stderr_redirects = 0;

    // always return true because kleene star
    return true;
}

bool synAndOr(list* l, token* arr, int* pos) {
    // check if none
    if (arr[*pos].type == NONE) {
        return false;
    }

    // at least one pipeline must be parsed
    if (!synPipeline(l, arr, pos)) {
        return false;
    }

    // now any extra pipelines may be parsed
    while (true) {
        // expect an && or ||
        if (arr[*pos].type != SEP_AND && arr[*pos].type != SEP_OR) {
            break;
        } else {
            lPush(l, &(arr[*pos]));

            // advance position
            (*pos)++;
        }

        // expect a stage to follow the pipe always
        if (!synPipeline(l, arr, pos)) {
            return false;
        }
    }

    // always return true because kleene star
    return true;
}

bool synList(list* l, token* arr, int* pos) {
    // check if none
    if (arr[*pos].type == NONE) {
        return false;
    }

    // at least one and_or must be parsed
    if (!synAndOr(l, arr, pos)) {
        return false;
    }

    // now any extra and_ors may be parsed
    while (true) {
        // expect an ; or &
        int backup_pos_list = *pos; 
        int backup_count_list = l->count;

        if (arr[*pos].type != SEP_END && arr[*pos].type != SEP_BG) {
            break;
        } else {
            lPush(l, &(arr[*pos]));
            // advance position
            (*pos)++;
        }

        // expect an andor to follow the ; or & always
        // otherwise, hope for the best with list ;
        if (!synAndOr(l, arr, pos)) {

            // delete as many as were added
            int current_count = l->count;
            for(int i = 0; i < current_count - backup_count_list; i++) {
                lDeleteLast(l);
            }

            // restore position
            *pos = backup_pos_list;

            return true;
        }
    }

    // always return true because kleene star
    return true;
}

bool synCommand(list* l, token* arr, int* pos) {
    // check if none
    if (arr[*pos].type == NONE) {
        return false;
    }

    // first, expect a list
    if(!synList(l, arr, pos)) {
        null_command = true;
        return false;
    }

    // then expect either a ; or a &
    if (arr[*pos].type == SEP_END || arr[*pos].type == SEP_BG) {
        // push onto list
        lPush(l, &(arr[*pos]));
        // advance posititon
        (*pos)++;
    } else {
        // if not, add one
        token* term = newTerminatorToken();
        lPush(l, term);
        freeToken(term);
    }

    // at this point always return true
    return true;
}

// * LIST DATA STRUCTURE

/**
 * @brief : initializes a stack-allocated list in place
 * It is an array of token structs
 *
 * @param l
 */
void lSetup(list* l) {
    // initialize list controller
    l->count = 0;
    l->cap = LIST_SIZE;
    l->tokens = malloc(sizeof(token) * l->cap);

    // initialize each token in the list
    for(int i = 0; i < l->cap; i++) {
        l->tokens[i].text = NULL;
        l->tokens[i].type = NONE;
    }
}

/**
 * @brief : frees heap-allocated components of list
 *
 * @param l
 */
void lDestroy(list* l) {
    for (int i = 0; i < l->count; i++) {
        emptyToken(&(l->tokens[i]));
    }
    // free the list of token structs
    free(l->tokens);
    // free controller
    free(l);
    // defensive set to NULL
    l = NULL;
}

/**
 * @brief : pushes a token to the end of list l
 *
 * @param l : the list being modified in place
 * @param tok : a pointer to the token being copied to the list
 */
void lPush(list* l, token* tok) {
    // check if pushing an item would require expansion
    if (l->count == l->cap) {
        lExpand(l);
    }
    // after expansion, we still want to continue adding the item
    if (l->count < l->cap) {
        // copy token type
        l->tokens[l->count].type = tok->type;
        if (tok->text) {
            // copy token text
            l->tokens[l->count].text = strdup(tok->text);
        }
        // increment count
        l->count++;
    }
}

/**
 * @brief : retrieves an item at a particular index from a list
 *
 * @param l : the list being inspected
 * @param index : the index from which to retrieve the item:
 * @return : ptr to list item if valid index, else return NULL
 */
token* lGet(list* l, int index) {
    if(index == - 1) {
        return &(l->tokens[l->count - 1]);
    } else if (index < l->count) {
        return &(l->tokens[index]);
    } else {
        return NULL;
    }
}

/**
 * @brief : doubles the size of the list when half-full
 *
 * @param l : the list to expand
 */
void lExpand(list* l) {
    // get new capacity
    int new_cap = l->cap * RESIZE_FACTOR;
    // reallocate
    token* new_tokens = realloc(l->tokens, new_cap * sizeof(token));
    // if reallocation success
    if (new_tokens != NULL) {
        // set the list controller to point to new tokens array
        l->tokens = new_tokens;
        // init the new tokens
        for(int i = l->cap; i < new_cap; i++) {
            l->tokens[i].type = NONE;
            l->tokens[i].text = NULL;
        }
        // set the list controller capacity
        l->cap = new_cap;
    }
}

/**
 * @brief : return the number of list_items in a list
 *
 * @param l : the list
 * @return int : the number of items
 */
int lCount(list* l) { 
    return l->count; 
}

list* lCopy(list* l) {
    // create a copy of the list
    list* copy = malloc(sizeof(list));

    // setup the list
    lSetup(copy);

    // push all the tokens from the original list to the new one
    for(int i = 0; i < l->count; i++) {
        lPush(copy, &(l->tokens[i]));
    }

    // return the duplicate list
    return copy;
}

void lDeleteNumber(list *l, int no) {
    // if nothing to delete, don't
    if (no == 0) {
        return;
    }

    // if everything to delete
    if (no >= l->count) {
        lDeleteAll(l);
    // if some number between 0 and l->count to delete
    } else {
        for (int i = 0; i < no; i++) {
            lDeleteLast(l);
        }
    }
}

void lDeleteAll(list *l) {
    while(l->count > 0) {
        lDeleteLast(l);
    }
}

void lDeleteLast(list* l) {
    token* last = lGet(l, -1);
    if (last != NULL) {
        emptyToken(last);
        (l->count)--;
    }
}

void lCap(list *l) {
    token* none = newNoneToken();
    lPush(l, none);
    freeToken(none);
}

/**
 * @brief : reorders the token array
 * 
 * @param l : the list containing the token array to reorder
 */
void lStageReorder(list *l, int start_of_stage) {
    // make a copy of the list
    list* copy = lCopy(l);

    // delete a number of tokens from the list
    lDeleteNumber(l, l->count - start_of_stage);

    int unmatched;
    // copy over all ARGS, LOCALs and RED_OPs in order
    for(int k = 0; k < 3; k++) {
        unmatched = 0;
        for (int i = start_of_stage; i < copy->count; i++) {
            if (unmatched > 0) {
                if (copy->tokens[i].type == PAR_LEFT) {
                    unmatched++;
                } else if (copy->tokens[i].type == PAR_RIGHT) {
                    unmatched--;
                }
            } else {
                if (copy->tokens[i].type == ARG && k == 0) {
                    lPush(l, &(copy->tokens[i]));
                } else if (copy->tokens[i].type == LOCAL && k == 1) {
                    lPush(l, &(copy->tokens[i]));
                } else if (RED_OP(copy->tokens[i].type) && k == 2) {
                    lPush(l, &(copy->tokens[i]));
                } else if (copy->tokens[i].type == PAR_LEFT) {
                    unmatched++;
                }
            }
        }
    }

    // copy the parenthesis stuff
    unmatched = 0;
    for (int i = start_of_stage; i < copy->count; i++) {
        if (unmatched > 0) {
            if (copy->tokens[i].type == PAR_LEFT) {
                unmatched++;
            } else if (copy->tokens[i].type == PAR_RIGHT) {
                unmatched--;
            }
            // copy all tokens
            lPush(l, &(copy->tokens[i]));
        } else {
            // only copy the beginning of a subcmd
            if (copy->tokens[i].type == PAR_LEFT) {
                lPush(l, &(copy->tokens[i]));
                unmatched++;
            }
        }
    }

    // free the duplicate list
    lDestroy(copy);
}

// * UTILITY FUNCTIONS

char* concat(char* s1, char* s2, char* mode) {
    char* result =
        malloc(strlen(s1) + strlen(s2) + 1);  // +1 for the null-terminator
    strcpy(result, s1);
    strcat(result, s2);
    if (strcmp(mode, "free") == 0) {
        free(s1);
        free(s2);
    }
    return result;
}

/**
 * @brief: Initialize a char array to a string with '/0'
 * 
 * @param arr : char array to initialize
 * @param len : length of the char array
 */
void initString(char* arr, int len) {
    for (int i = 0; i < len; i++) {
        arr[i] = '\0';
    }
}

bool isSimple(char c) {
    for (int i = 0; i < strlen(METACHAR); i++) {
        // if any of the METACHARS
        if (c == METACHAR[i]) {
            // return false
            return false;
        }
    }
    // if non of the METACHARS, return true
    return true;
}

/**
 * @brief : check if the character is in VARCHR
 * 
 * @param c : the character to check
 * @return true : if it is in VARCHR
 * @return false : if not
 */
bool charInVarChr(char c) {
    for (int i = 0; i < strlen(VARCHR); i++) {
        if (c == VARCHR[i]) {
            return true;
        }
    }
    return false;
}

// * TOKEN FUNCTIONS

/**
 * @brief : returns a copy of a token
 * 
 * @param src : the token to copy
 * @return token* : ptr to the duplicate token
 */
token* newTokenFrom(token* src) {
    token* new = malloc(sizeof(token));
    copyToken(new, src);
    return new;
}

/**
 * @brief : copies a token from src into dest
 * 
 * @param dest : a ptr to a token struct
 * @param src : a ptr to a token struct
 */
void copyToken(token* dest, token* src) {
    // set the token type
    dest->type = src->type;
    dest->text = NULL;
    // copy the text
    if (src->text) {
        dest->text = strdup(src->text);
    }
}

/**
 * @brief : frees the heap-allocated components of a token
 * 
 * @param t : ptr to the token to free
 */
void freeToken(token* t) {
    emptyToken(t);
    free(t);
}

/**
 * @brief : frees the heap allocated components of a token
 * BUT NOT the token struct itself
 * 
 * @param t : the token to empty
 */
void emptyToken(token *t) {
    if (t != NULL) {
        // set type to NONE
        t->type = NONE;

        // free the text
        if (t->text) {
            free(t->text);
        }

        // set text ptr to NULL
        t->text = NULL;
    }
}

/**
 * @brief : return a new token which is an error type
 * 
 * @return token* : ptr to the new token 
 */
token* newErrorToken(void) {
    token* err = malloc(sizeof(token));
    err->text = NULL;
    err->type = ERROR;
    return err;
}

/**
 * @brief : return a new token which is a none type
 * 
 * @return token* : ptr to the new token
 */
token* newNoneToken(void) {
    token* none = malloc(sizeof(token));
    none->text = NULL;
    none->type = NONE;
    return none;
}

/**
 * @brief : return a new token which is a none type
 *
 * @return token* : ptr to the new token
 */
token* newTerminatorToken(void) {
    token* term = malloc(sizeof(token));
    term->text = NULL;
    term->type = SEP_END;
    return term;
}

/**
 * @brief : convert an integer type of token to a string
 * 
 * @param type : the type as specified in parsnip.h
 * @return char* : the corresponding string
 */
char* printTokenType(int type) {
    char* str;
    switch (type) {
        case SIMPLE:
            str = "SIMPLE";
            break;
        case RED_IN:
            str = "RED_IN";
            break;
        case RED_IN_HERE:
            str = "RED_IN_HERE";
            break;
        case RED_OUT:
            str = "RED_OUT";
            break;
        case RED_OUT_APP:
            str = "RED_OUT_APP";
            break;
        case RED_ERR_OUT:
            str = "RED_ERR_OUT";
            break;
        case RED_ERR_CLS:
            str = "RED_ERR_CLS";
            break;
        case PIPE:
            str = "PIPE";
            break;
        case SEP_AND:
            str = "SEP_AND";
            break;
        case SEP_OR:
            str = "SEP_OR";
            break;
        case SEP_END:
            str = "SEP_END";
            break;
        case SEP_BG:
            str = "SEP_BG";
            break;
        case PAR_LEFT:
            str = "PAR_LEFT";
            break;
        case PAR_RIGHT:
            str = "PAR_RIGHT";
            break;
        case NONE:
            str = "NONE";
            break;
        case ERROR:
            str = "ERROR";
            break;
        case SUBCMD:
            str = "SUBCMD";
            break;
        case LOCAL:
            str = "LOCAL";
            break;
        case ARG:
            str = "ARG";
            break;
        default:
            str = "ERROR: UNKNOWN TYPE";
            break;
    }
    return str;
}
