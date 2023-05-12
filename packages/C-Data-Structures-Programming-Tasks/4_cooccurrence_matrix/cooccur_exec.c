#define _GNU_SOURCE

#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "cooccur.h"
#include "smap.h"

// prototypes for functions, full documentation with definitions below
// int djb2(const char *key);
int normal_string_compare(const void *s1, const void *s2);
// void reset_order(const char* key, int* order);
// void free_value(const char *key, int *value);
void print_vector(FILE *stream, const char* key, double* vector, int size);

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("Cooccur: atleast two additional arguments required to test for coccurence\n");
        return 1;
    }

    // start at the second command line argument
    int keyword_count = argc - 1;
    char** keywords = &argv[1];

    // create the matrix
    cooccurrence_matrix* calculate = cooccur_create(keywords, keyword_count);

    // read in contexts
    // stripped_size to check EOF when -1
    // stripped to check for empty arrays when NULL
    int stripped_size = 0;
    char** stripped = NULL;

    while(true)
    {
        stripped = cooccur_read_context(calculate, stdin, &stripped_size);

        if(stripped_size == -1)
        {
            free(stripped);
            break;
        }

        if(stripped != NULL)
        {
            cooccur_update(calculate, stripped, stripped_size);

            for(int i = 0; i < stripped_size; i++)
            {
                free(stripped[i]);
            }
            free(stripped);
        }
    }

    // get and print the vectors
    for(int i = 0; i < keyword_count; i++)
    {
        double* vector = cooccur_get_vector(calculate, keywords[i]);
        print_vector(stdout, keywords[i], vector, keyword_count);
        free(vector);
    }

    // works well, but still getting possible memory loss from getline()
    cooccur_destroy(calculate);

    return 0;
}

/**
 * an smap_for_each helper
 * @param key   unused
 * @param order the order/value associated with the key
 */
/*
void free_value(const char *key, int *value)
{
  free(value);
}
*/

/**
 * an smap_for_each helper
 * @param key   unused
 * @param order the order/value associated with the key
 */
/*
void reset_order(const char* key, int* order)
{
    order[0] = 0;
}
*/

/**
 * taken from http://www.cse.yorku.ca/~oz/hash.html / djb2 hash
 * @param  key string to be hashed
 * @return     an int output
 */
/*
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
*/

/**
 * helps qsort strings
 * @param  s1
 * @param  s2
 */
int normal_string_compare(const void *s1, const void *s2)
{
  const char * const *a = s1;
  const char * const *b = s2;

  return strcmp(*a, *b);
}

/**
 * a vector pretty-print
 * @param stream output stream
 * @param key    keyword associated with vector
 * @param vector an array of doubles
 * @param size   size of the array
 */
void print_vector(FILE *stream, const char* key, double* vector, int size)
{
    fprintf(stream, "%s: [", key);
    for(int i = 0; i < size; i++)
    {
        if(i == size - 1)
        {
            fprintf(stream, "%f]\n", vector[i]);
            break;
        }
        else
        {
            fprintf(stream, "%f, ", vector[i]);
        }
    }
}
