#include "process-stub.h"
#include "parsnip.h"

int main(void) {

    // char* args[2];
    // args[0] = "ls";
    // args[1] = NULL;
    // execvp("ls", args);
    // return 0;

    char tok[] = "folder=hidden";
    char* name = strtok(tok, "=");
    printf("%s\n", name);
    // printf("%s\n", tok);

    char* val = strtok(NULL, "=");
    
    printf("%s\n", val);
}