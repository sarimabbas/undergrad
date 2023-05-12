#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "chomp.h"
#include "string_helpers.h"
#include "smap.h"

enum {CURRENT, OTHER};

typedef enum
{
    WIN,
    LOSS,
    UNDEFINED
} conditions;

int djb2(const char *key);
void free_value(const char *key, int *value);

int main(int argc, char **argv)
{
    if (argc < 2 || strlen(argv[1]) == 0 || argv[1][0] == '0')
    {
        fprintf(stderr, "USAGE: %s state\n", argv[0]);
        return 1;
    }

    int cols = strlen(argv[1]);
    int rows = 1;
    for (char *s = argv[1]; *s; s++)
    {
        if (!isdigit(*s) || *s < *(s + 1))
        {
            // string contains non-digit or increasing digits
            fprintf(stderr, "USAGE: %s state\n", argv[0]);
            return 1;
        }

        // rows is max over all digits in string
        int h = *s - '0';
        if (h > rows)
        {
            rows = h;
        }
    }

    int count;
    char **states = chomp_states(rows, cols, &count);
    if (states == NULL)
    {
        fprintf(stderr, "%s: error allocating memory\n", argv[0]);
        return 1;
    }

    // create a map from states (strings) to position values
    smap* state_map = smap_create(djb2);

    // set final state to win for current player
    int* final_state = malloc(sizeof(int) * 1);
    *final_state = WIN;
    smap_put(state_map, states[0], final_state);

    // for each other state
    // all the states after the first state
    for (int i = 1; i < count; i++)
    {
        // get successor states
        int succ_count;
        char **succ = chomp_successors(states[i], &succ_count);
        if (succ != NULL)
        {
            // if all successor states are wins then set states[i] to loss
            // else set states[i] to win
            bool all_wins = true;

            for(int j = 0; j < succ_count; j++)
            {
                int* current_condition = smap_get(state_map, succ[j]);
                if(*current_condition != WIN)
                {
                    all_wins = false;
                    break;
                }
            }

            int* end_game = malloc(sizeof(int) * 1);
            if(all_wins)
            {
                *end_game = LOSS;
                smap_put(state_map, states[i], end_game);
            }
            else
            {
                *end_game = WIN;
                smap_put(state_map, states[i], end_game);
            }
        }

        string_array_free(succ, succ_count);
    }

    // look up result for argv[1]
    int* result = smap_get(state_map, argv[1]);

    // if a loss then print LOSS
    if(*result == LOSS)
    {
        printf("LOSS\n");
    }
    // else find first successor that is a win and output WIN: that successor
    else
    {
        // get successors of argv[1]
        int succ_count;
        char **succ = chomp_successors(argv[1], &succ_count);
        if(succ != NULL)
        {
            // for(int m = succ_count - 1; m >= 0; m--)
            for(int m = 0; m < succ_count; m++)
            {
                int* current_condition = smap_get(state_map, succ[m]);
                if(*current_condition == LOSS)
                {
                    printf("WIN: %s\n", succ[m]);
                    break;
                }
            }
            string_array_free(succ, succ_count);
        }
    }

    string_array_free(states, count);
    smap_for_each(state_map, free_value);
    smap_destroy(state_map);
    return 0;
}

/**
 * taken from http://www.cse.yorku.ca/~oz/hash.html / djb2 hash
 * @param  key string to be hashed
 * @return     an int output
 */

int djb2(const char *key)
{
    int hash = 5381;
    int c;
    while ((c = *key++))
    {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

/**
 * an smap_for_each helper
 * @param key   unused
 * @param order the order/value associated with the key
 */
void free_value(const char *key, int *value)
{
  free(value);
}
