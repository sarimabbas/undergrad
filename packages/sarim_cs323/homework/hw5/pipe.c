// pipe.c                                         Stan Eisenstat (11/06/18)
// Execute a chain of filters that do not take command line arguments; e.g.,
// "pipe ls wc" is equivalent to "ls | wc"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// Print error message and die with STATUS
#define ERROREXIT(status) (perror("pipe"), exit(status))

// Extract status from value returned by waitpid(); ensure that a process
// that is killed has nonzero status; ignores the possibility of stop/continue.
#define STATUS(x) (WIFEXITED(x) ? WEXITSTATUS(x) : 128 + WTERMSIG(x))

int main(int argc, char *argv[]) {
    struct entry {  // Table with (pid,status) for each child
        int pid, status;
    } table[argc];
    int fd[2],        // Read and write file descriptors for pipe
        pid, status,  // Process ID and status of child
        fdin,         // Read end of last pipe (or -1 if none)
        i, j;

    if (argc < 2) {
        printf("Usage:  pipe filter1 filter2 ... filterN\n");
        return EXIT_SUCCESS;
    }

    fdin = -1;
    for (i = 1; i < argc - 1; i++) {  // Create chain of processes
        printf("%i\n", argc);
        exit(1);
        if (pipe(fd) || (pid = fork()) < 0)
            ERROREXIT(EXIT_FAILURE);

        else if (pid == 0) {  // Child process
            close(fd[0]);     //  No reading from new pipe
            if (fdin >= 0) {  //  stdin = read[last pipe]
                dup2(fdin, 0);
                close(fdin);
            }
            if (fd[1] != 1) {  //  stdout = write[new pipe]
                dup2(fd[1], 1);
                close(fd[1]);
            }
            execlp(argv[i], argv[i], NULL);  //  Overlay by i-th filter
            ERROREXIT(EXIT_FAILURE);

        } else {                 // Parent process
            table[i].pid = pid;  //  Save child pid
            if (fdin >= 0)       //   Close read[last pipe]
                close(fdin);
            fdin = fd[0];  //  Remember read[new pipe]
            close(fd[1]);  //  No writing to new pipe
        }
    }

    /*888888888888888888888888888888888888888888*/

    if ((pid = fork()) < 0)  // Create last process
        ERROREXIT(EXIT_FAILURE);

    else if (pid == 0) {  // Child process
        if (fdin >= 0) {  //  stdin = read[last pipe]
            dup2(fdin, 0);
            close(fdin);
        }
        execlp(argv[argc - 1], argv[argc - 1], 0);  //  Overlay by last filter
        ERROREXIT(EXIT_FAILURE);

    } else {                        // Parent process
        table[argc - 1].pid = pid;  //  Save child pid
        if (fdin >= 0)              //  Close read[last pipe]
            close(fdin);
    }

    signal(SIGINT, SIG_IGN);  // Ignore interrupts

    for (i = 1; i < argc;) {  // Wait for children to die
        pid = waitpid(-1, &status, 0);
        for (j = 1; j < argc && table[j].pid != pid; j++)
            ;
        if (j < argc) {  // Ignore zombie processes
            table[j].status = status;
            i++;
        }
    }

    for (i = 1; i < argc; i++) {  // Print information
        printf("%-10s\tpid=%d  status = %-3d  signal=%-3d  exit=%-3d\n",
               argv[i], table[i].pid, STATUS(table[i].status),
               WTERMSIG(table[i].status), WEXITSTATUS(table[i].status));
    }

    return EXIT_SUCCESS;
}
