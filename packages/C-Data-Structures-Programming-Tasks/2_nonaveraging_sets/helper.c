#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "helper.h"

#define ARRAY_PLACEHOLDER -1

char modAtoi(char* string, int* store)
{
    if(strcmp(string, "0") == 0)
    {
        *store = 0;
        return 's'; // success!
    }
    else
    {
        *store = atoi(string);
        if(*store) // if atoi succeeds on numbers
        {
            if(*store < 0)
            {
                return 'n'; // negative!
            }
            else
            {
                return 's'; // success!
            }
        }
        else // if atoi fails e.g. letters
        {
            return 'e'; // not a number!
        }
    }
}

bool flipArray(int* array, int* flip_array, int array_size)
{
    int flip_array_position = 0;
    for(int i = array_size - 1; i >= 0; i--)
    {
        flip_array[flip_array_position] = array[i];
        flip_array_position = flip_array_position + 1;
    }
    return true;
}

bool arrayCopy(int* array_source, int* array_destination, int array_size)
{
    for(int i = 0; i < array_size; i++)
    {
        array_destination[i] = array_source[i];
    }
    return true;
}

bool bubbleSortArray(int* array, int array_size)
{
	for(int x = 0; x < array_size; x++)
	{
		for(int y = 0; y < array_size - 1; y++)
		{
			if(array[y] > array[y + 1])
			{
				int temp = array[y + 1];
				array[y + 1] = array[y];
				array[y] = temp;
			}
		}
	}
    return true;
}

bool arrayPopulate(int* array, int array_size)
{
    for(int i = 0; i < array_size; i++)
    {
        array[i] = i;
    }
    return true;
}

bool emptyArrayPopulate(int* array, int array_size)
{
    for(int i = 0; i < array_size; i++)
    {
        array[i] = ARRAY_PLACEHOLDER;
    }
    return true;
}

bool arrayPrint(int* array, int array_size, char method, int first, int step, char empty)
{
    // compute k / size of set
    int k = 0;
    for(int i = 0; i < array_size; i++)
    {
        if(array[i] != ARRAY_PLACEHOLDER)
        {
            k = k + 1;
        }
    }

    // print the method
    if(method == 'g')
    {
        printf("-greedy");
    }
    else if(method == 'b')
    {
        printf("-backward");
    }
    else if(method == 's')
    {
        printf("-skip %i %i", first, step);
    }
    else if(method == 'o')
    {
        printf("-opt");
    }
    else
    {
        printf("-unspecified");
    }

    // useful if we have a must include set with arithmetic progressions
    if(empty == 'y')
    {
        printf(": 0 []\n");
    }
    else
    {
        // print k
        printf(": %i", k);
        // print set
        printf(" [");
        for(int i = 0; i < array_size; i++)
        {
            // if the element is not a placeholder
            if(array[i] != ARRAY_PLACEHOLDER)
            {
                // if the element is the last
                if(i == array_size - 1)
                {
                    // print without spaces
                    printf("%i", array[i]);
                }
                // if the element is not the last
                else
                {
                    // if the element after it is a placeholder
                    if(array[i + 1] == ARRAY_PLACEHOLDER)
                    {
                        printf("%i", array[i]);
                    }
                    else
                    {
                        printf("%i, ", array[i]);
                    }
                }
            }
        }
        printf("]\n");
    }
    return true;
}

bool debugTruePrint(int* array, int array_size)
{
    printf("debugTruePrint: [");
    for(int i = 0; i < array_size; i++)
    {
        if(i == array_size - 1)
        {
            printf("%i", array[i]);
        }
        else
        {
            printf("%i, ", array[i]);
        }
    }
    printf("]\n");
    return true;
}

bool mustIncludeModifier(int* must_include, int must_include_size,
    int* available_numbers, int* progression_free)
{
    int largest = -1;
    for(int i = 0; i < must_include_size; i++)
    {
        if(must_include[i] == ARRAY_PLACEHOLDER)
        {
            continue;
        }

        progression_free[i] = must_include[i];

        // find the largest
        if(must_include[i] > largest)
        {
            largest = must_include[i];
        }
    }
    // now we know the largest integer from the must include set
    // we use this to modify the initial available numbers set
    // e.g. available_numbers = {0, 1, 2, 3, 4, 5, 6}
    // must_include_array = {4, 5}
    // available_numbers = {6}
    for(int i = 0; i <= largest; i++)
    {
        available_numbers[i] = ARRAY_PLACEHOLDER;
    }
    return true;
}

bool greedy(int* progression_free, int progression_free_size,
    int* available_numbers, int available_numbers_size)
{
    makeProgressionFree(progression_free, progression_free_size,
        available_numbers, available_numbers_size,
        0, 1, 'n');
    return true;
}

bool backward(int* progression_free, int progression_free_size,
    int* available_numbers, int available_numbers_size)
{
    int flip_available_numbers[available_numbers_size];
    flipArray(available_numbers, flip_available_numbers, available_numbers_size);
    makeProgressionFree(progression_free, progression_free_size,
        flip_available_numbers, available_numbers_size,
        0, 1, 'n');
    return true;
}

bool skip(int* progression_free, int progression_free_size,
    int* available_numbers, int available_numbers_size,
    int start, int step)
{
    makeProgressionFree(progression_free, progression_free_size,
        available_numbers, available_numbers_size,
        start, step, 'y');
    return true;
}

bool makeProgressionFree(int* progression_free, int progression_free_size,
    int* available_numbers, int available_numbers_size,
    int start, int step, char wrap)
{
    // compute where to start pushing numbers
    // the position must begin after the must include numbers, if any
    int progression_free_position = 0;
    for(int i = 0; i < progression_free_size; i++)
    {
        if(progression_free[i] == ARRAY_PLACEHOLDER)
        {
            progression_free_position = i;
            break;
        }
    }

    bool progression_caused = false;
    bool repeat = false;
    // looking at available choices
    for(int i = start; i < available_numbers_size;
        (wrap == 'y') ? (i = (i + step) % available_numbers_size) : i++)
    {
        if(available_numbers[i] == ARRAY_PLACEHOLDER)
        {
            continue;
        }

        if(i == start && repeat)
        {
            break;
        }
        repeat = true;

        // looking at progression_free
        for(int j = 0; j < progression_free_size - 1; j++)
        {
            // we don't want to keep comparing to placeholders
            if(progression_free[j] == ARRAY_PLACEHOLDER)
            {
                continue;
            }

            // looking at progression_free
            for(int k = j + 1; k < progression_free_size; k++)
            {
                // we don't want to keep comparing to placeholders
                if(progression_free[k] == ARRAY_PLACEHOLDER)
                {
                    continue;
                }

                // compute differences
                int pair_difference = abs(progression_free[j] - progression_free[k]);
                int newcomer_difference = abs(available_numbers[i] - progression_free[k]);
                int extreme_difference = abs(available_numbers[i] - progression_free[j]);
                if(pair_difference == newcomer_difference ||
                    newcomer_difference == extreme_difference ||
                    pair_difference == extreme_difference)
                {
                    progression_caused = true;
                    break;
                }
            }
            // we don't want to keep comparing if progression caused for some number i
            if(progression_caused)
            {
                break;
            }
        }
        if(progression_caused)
        {
            // reset the progression flag
            progression_caused = false;
        }
        else
        {
            // add the integer to the array
            progression_free[progression_free_position] = available_numbers[i];
            progression_free_position = progression_free_position + 1;
        }
    }
    bubbleSortArray(progression_free, progression_free_size);
    return true;
}


bool hasProgression(int* array, int array_size)
{
    // pick a number/choice
    for(int i = 0; i < array_size; i++)
    {
        // we don't want to keep comparing to placeholders
        if(array[i] == ARRAY_PLACEHOLDER)
        {
            continue;
        }
        int choice = array[i];

        // make an array without this choice e.g. {5, 6, 7} => {-1, 6, 7}
        int rest_array[array_size];
        arrayCopy(array, rest_array, array_size);
        rest_array[i] = ARRAY_PLACEHOLDER;

        for(int j = 0; j < array_size - 1; j++)
        {
            // we don't want to keep comparing to placeholders
            if(rest_array[j] == ARRAY_PLACEHOLDER)
            {
                continue;
            }
            for(int k = j + 1; k < array_size; k++)
            {
                // we don't want to keep comparing to placeholders
                if(rest_array[k] == ARRAY_PLACEHOLDER)
                {
                    continue;
                }
                // compute differences
                int pair_difference = abs(rest_array[j] - rest_array[k]);
                int newcomer_difference = abs(choice - rest_array[k]);
                int extreme_difference = abs(choice - rest_array[j]);

                if(pair_difference == newcomer_difference ||
                    newcomer_difference == extreme_difference ||
                    pair_difference == extreme_difference)
                {
                    return true;
                }
            }
        }
    }
    return false;
}
