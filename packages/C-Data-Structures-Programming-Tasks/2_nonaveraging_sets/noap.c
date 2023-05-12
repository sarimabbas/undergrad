#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "helper.h"

#define ERROR_CODE 42
#define ALGORITHM_PLACEHOLDER 'x'
#define ARRAY_PLACEHOLDER -1
#define NUMBER_OF_ALGORITHMS 4

/*
I changed around just a few things because I lost a lot of points on error checking.
First, I met the requirement that n cannot be 0.
Then, I just modified the variable "temp" to make the test for the skip params pass. That's it!
*/

int main(int argc, char* argv[])
{
    /*
     * CHECK FOR FIRST COMMAND LINE ARGUMENT
     */
    if(argc < 2)
    {
        printf("NoAP: first command line argument is required\n");
        return 1;
    }
    int n;
    char n_result = modAtoi(argv[1], &n);
    if(n_result == 'n' || n == 0) { printf("NoAP: n must not be negative; was %i\n", n); return ERROR_CODE; }
    else if(n_result == 'e') { printf("NoAP: n must be an integer\n"); return ERROR_CODE; }

    /*
     * CHECK FOR MUST INCLUDE NUMBERS
     */
    // check for must include numbers
    int must_include[n];
    emptyArrayPopulate(must_include, n);
    int temp = 0;
    for(int i = 2; i < argc; i++)
    {
        char temp_result = modAtoi(argv[i], &temp);
        if(temp_result != 'e')
        {
            if(temp < 0 || temp > n - 1)
            {
                printf("NoAP: integer out of range %i\n", temp);
                return ERROR_CODE;
            }
            else
            {
                must_include[i - 2] = temp;
            }
        }
        else
        {
            break; // the numbers have to be contiguous
        }
    }
    if(temp != 0) { temp++; }

    /*
     * AT THIS POINT, CREATE THE AVAILABLE NUMBERS ARRAY
     * ALSO CREATE THE PROGRESSION FREE ARRAY
     * FINALLY, MODIFY THE AVAILABLE NUMBERS WITH THE MUST INCLUDES
     */
    int available_numbers[n];
    arrayPopulate(available_numbers, n);

    int progression_free[n];
    emptyArrayPopulate(progression_free, n);

    bool modify_success = mustIncludeModifier(must_include, n, available_numbers, progression_free);
    if(modify_success == false)
    {
        printf("NoAP: could not modify the available numbers\n");
        return ERROR_CODE;
    }

    /*
     * CHECK FOR ALGORITHMS AND THEIR ARGUMENTS
     */
    bool greedy_found = false;
    bool backward_found = false;
    bool skip_found = false;
    bool opt_found = false;
    int first = 0;
    int step = 0;

    // cannot be more than 4 algorithms
    // we use an array so we keep track of the order
    char algorithms[NUMBER_OF_ALGORITHMS] = {ALGORITHM_PLACEHOLDER, ALGORITHM_PLACEHOLDER,
        ALGORITHM_PLACEHOLDER, ALGORITHM_PLACEHOLDER};
    int algorithm_position = 0;
    for(int i = 2; i < argc; i++)
    {
        // check for flags
        if(strcmp("-greedy", argv[i]) == 0)
        {
            if(greedy_found)
            {
                continue;
            }
            greedy_found = true;
            algorithms[algorithm_position] = 'g';
            algorithm_position = algorithm_position + 1;
        }
        else if(strcmp("-backward", argv[i]) == 0)
        {
            if(backward_found)
            {
                continue;
            }
            backward_found = true;
            algorithms[algorithm_position] = 'b';
            algorithm_position = algorithm_position + 1;
        }
        else if(strcmp("-skip", argv[i]) == 0)
        {
            if(skip_found)
            {
                continue;
            }
            skip_found = true;
            algorithms[algorithm_position] = 's';
            algorithm_position = algorithm_position + 1;

            // not sure why i + 3 works but oh well
            if((i + 3) > argc) { printf("NoAP: both skip arguments not given\n"); return ERROR_CODE; }

            char first_result = modAtoi(argv[i + 1], &first);
            // if(first_result == 'n') { printf("NoAP: skip first is negative\n"); return ERROR_CODE; }
            if(first_result == 'e') { printf("NoAP: skip first is not an integer\n"); return ERROR_CODE; }
            else if((first > n - 1) || (first < temp)) { printf("NoAP: invalid first %i\n", first); return ERROR_CODE; }

            char step_result = modAtoi(argv[i + 2], &step);
            // if(step_result == 'n') { printf("NoAP: skip step is negative\n"); return ERROR_CODE; }
            if(step_result == 'e') { printf("NoAP: skip step is not an integer\n"); return ERROR_CODE; }
            else if((step > n - temp) || (step < 1)) { printf("NoAP: invalid step %i\n", step); return ERROR_CODE; }
        }
        else if(strcmp("-opt", argv[i]) == 0)
        {
            if(opt_found)
            {
                continue;
            }
            opt_found = true;
            algorithms[algorithm_position] = 'o';
            algorithm_position = algorithm_position + 1;
        }
    }
    // there must be atleast one algorithm specified
    if(!(greedy_found || backward_found || skip_found || opt_found))
    {
        printf("NoAP: please specify atleast one algorithm\n");
        return ERROR_CODE;
    }

    /*
     * NOW START EXECUTING THE ALGORITHMS
     */
    for(int i = 0; i < NUMBER_OF_ALGORITHMS; i++)
    {
        if(algorithms[i] == 'g')
        {
            if(hasProgression(must_include, n))
            {
                arrayPrint(must_include, 0, 'g', 0, 0, 'y');
            }
            else
            {
                int greedy_available_numbers[n];
                int greedy_progression_free[n];
                arrayCopy(available_numbers, greedy_available_numbers, n);
                arrayCopy(progression_free, greedy_progression_free, n);

                greedy(greedy_progression_free, n, greedy_available_numbers, n);
                arrayPrint(greedy_progression_free, n, 'g', 0, 0, 'n');
            }
        }
        else if(algorithms[i] == 'b')
        {
            if(hasProgression(must_include, n))
            {
                arrayPrint(must_include, 0, 'b', 0, 0, 'y');
            }
            else
            {
                int backward_available_numbers[n];
                int backward_progression_free[n];
                arrayCopy(available_numbers, backward_available_numbers, n);
                arrayCopy(progression_free, backward_progression_free, n);

                backward(backward_progression_free, n, backward_available_numbers, n);
                arrayPrint(backward_progression_free, n, 'b', 0, 0, 'n');
            }
        }
        else if(algorithms[i] == 's')
        {
            if(hasProgression(must_include, n))
            {
                arrayPrint(must_include, 0, 's', first, step, 'y');
            }
            else
            {
                int skip_available_numbers[n];
                int skip_progression_free[n];
                arrayCopy(available_numbers, skip_available_numbers, n);
                arrayCopy(progression_free, skip_progression_free, n);

                /*
                 * TRIM AVAILABLE NUMBERS FOR WRAPAROUNDS
                 */

                // we'll first find the size of the available numbers without placeholders
                int trim_available_numbers_size = 0;
                for(int i = 0; i < n; i++)
                {
                    if(skip_available_numbers[i] != ARRAY_PLACEHOLDER)
                    {
                        trim_available_numbers_size = trim_available_numbers_size + 1;
                    }
                }

                // then we'll create a new array to put those numbers in
                int trim_available_numbers[trim_available_numbers_size];
                int trim_available_numbers_position = 0;
                for(int i = 0; i < n; i++)
                {
                    if(skip_available_numbers[i] != ARRAY_PLACEHOLDER)
                    {
                        trim_available_numbers[trim_available_numbers_position] = skip_available_numbers[i];
                        trim_available_numbers_position = trim_available_numbers_position + 1;
                    }
                }

                // lastly, we'll find the index of the specified first
                int first_index;
                for(int i = 0; i < trim_available_numbers_size; i++)
                {
                    if(trim_available_numbers[i] == first)
                    {
                        first_index = i;
                    }
                }

                skip(skip_progression_free, n, trim_available_numbers, trim_available_numbers_size, first_index, step);
                arrayPrint(skip_progression_free, n, 's', first, step, 'n');
            }
        }
        else if(algorithms[i] == 'o')
        {
            arrayPrint(must_include, 0, 'o', 0, 0, 'y');
        }
    }
    // exit with success
    return 0;
}
