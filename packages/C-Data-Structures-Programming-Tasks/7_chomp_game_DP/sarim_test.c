#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "chomp.h"
#include "string_helpers.h"
#include "smap.h"

int main(int argc, char **argv)
{
    // CHOMP STATES
    /*
    int state_size;
    char** states = chomp_states(4, 4, &state_size);
    for(int i = 0; i < state_size; i++)
    {
        printf("%s\n", states[i]);
    }
    */

    /*
    printf("ASCII test:\n");
    char four = '4';
    printf("here is a char 4: %c\n", four);
    printf("here is the char in ascii: %i\n", (int) four);
    printf("here is the char to an int literal: %i\n", (int) (four - '0'));
    int five = 5;
    printf("here is an int 5: %i\n", five);
    printf("here is the int in ascii %i\n", (int) (five + '0'));
    printf("here is the int as a char literal: %c\n", (char) (five + '0'));
    */

    // CHOMP SUCCESSORS
    char* sample_state = "22221";
    int successors_size;
    char** successors = chomp_successors(sample_state, &successors_size);
    printf("successors size: %i\n", successors_size);
    for(int i = 0; i < successors_size; i++)
    {
        printf("%s\n", successors[i]);
        free(successors[i]);
    }
    free(successors);

    return 0;
}
