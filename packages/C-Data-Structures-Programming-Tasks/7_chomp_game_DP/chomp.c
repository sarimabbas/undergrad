#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "chomp.h"

int min(int one, int two);
int char_to_int_literal(char c);
char int_to_char_literal(int i);

char **chomp_states(int rows, int cols, int *n)
{
    if (rows <= 0 || cols <= 0 || rows > 9)
    {
        return NULL;
    }

    // count[i][j] is the number of rows*(i+1) chomp states with exactly j in the last column
    int count[cols + 1][rows + 1];
    for (int r = 0; r <= rows; r++)
    {
        count[0][r] = 1;
    }

    for (int c = 2; c <= cols + 1; c++)
    {
        for (int r = 0; r <= rows; r++)
        {
            int sum = 0;
            for (int r2 = r; r2 <= rows; r2++)
            {
                sum += count[c - 2][r2];
                if (sum <= 0)
                {
                    // overflow
                    return NULL;
                }
            }
            count[c - 1][r] = sum;
        }
    }

    *n = count[cols][0];

    char **result = malloc(sizeof(char *) * *n);
    if (result != NULL)
    {
        // last state is whole pan
        result[*n - 1] = malloc(sizeof(char) * (cols + 1));
        if (result[*n - 1] != NULL)
        {
            for (int i = 0; i < cols; i++)
            {
                result[*n - 1][i] = '0' + rows;
            }
            result[*n - 1][cols] = '\0';
        }

        // each previous state is obtained by decrementing the next
        for (int i = *n - 2; i >= 0; i--)
        {
            if (result[i + 1] != NULL)
            {
                result[i] = malloc(sizeof(char) * (cols + 1));
                if (result[i] != NULL)
                {
                    strcpy(result[i], result[i + 1]);
                    // find last non-zero (know there will be one)
                    int j = cols - 1;
                    while (result[i][j] == '0')
                    {
                        j--;
                    }

                    // decrement it
                    result[i][j]--;

                    // set all subsequent to equal it
                    for (int k = j + 1; k < cols; k++)
                    {
                        result[i][k] = result[i][j];
                    }
                }
            }
        }
    }
    else
    {
        *n = 0;
    }

    return result;
}

char **chomp_successors(const char *s, int *n)
{
    if(s == NULL)
    {
        return NULL;
    }

    // add up all digits in the state
    int state_length = strlen(s);
    int running_total = 0;
    for(int i = 0; i < state_length; i++)
    {
        running_total = running_total + char_to_int_literal(s[i]);
    }

    // malloc space for new array and set it
    char** array = malloc(sizeof(char*) * running_total);
    if(array == NULL) { return NULL; }

    // create a counter variable
    int how_many_states = 0;

    // for each column, going left to right
    for(int i = 0; i < state_length; i++)
    {
        // for each row, going bottom up
        for(int j = 0; j < char_to_int_literal(s[i]); j++)
        {
            // construct a state
            char* new_state = malloc(sizeof(char) * (state_length + 1));
            if(new_state == NULL) { return NULL; }

            // for all columns less than the current one
            for(int k = 0; k < i; k++)
            {
                // they have the same number of brownies
                new_state[k] = s[k];
            }

            // for all columns after the current one
            for(int k = i; k < state_length; k++)
            {
                // find the minimum of the current column digit and the brownies left
                int minimum = min(j, char_to_int_literal(s[k]));
                new_state[k] = int_to_char_literal(minimum);
            }

            // for the current column, it gets j brownies
            new_state[i] = int_to_char_literal(j);

            // attach the new state to our array
            new_state[state_length] = '\0';
            array[how_many_states] = new_state;

            // increment the size
            how_many_states++;
        }
    }

    *n = how_many_states;
    return array;
}

int min(int one, int two)
{
    if(one < two)
    {
        return one;
    }
    else if(two < one)
    {
        return two;
    }
    else
    {
        return one;
    }
}

/**
 * Use this instead of atoi to keep sanity
 * e.g. '4' => 52 => 4
 * @param  c a char
 * @return   an int
 */
int char_to_int_literal(char c)
{
    return (int) (c - '0');
}

/**
 * Use this instead of atoi to keep sanity
 * e.g. 4 => 52 => '4'
 * @param  c an int
 * @return   a char
 */
char int_to_char_literal(int i)
{
    return (char) (i + '0');
}
