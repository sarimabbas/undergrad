/*
 * Sarim Abbas
 * sarim.abbas@yale.edu
 *
 * Pset 4 Resubmission
 *
 * What changed (mostly the stuff needed to run the timing test cases):
 * cooccur.c:               removed main, removed stray comments, removed some private functions
 * cooccur_exec.c:          new file, added main, added some private functions
 * smap.c:                  slightly changed smap_compute_index, so it returns an unsigned index into array
 *                          slightly changed smap_embiggen to fix valgrind errors for uninitialized values
 * makefile:                added rules to create SmapUnit and CooccurUnit, as well as changed rules for Cooccur
 * smap_test_functions.c:   Prof. Glenn's file
 * cooccur_unit.c:          Prof. Glenn's file
 *
 */

#define _GNU_SOURCE

#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "cooccur.h"
#include "smap.h"

struct cooccurrence_matrix
{
  smap* keyword_vectors;
  smap* keyword_orders;
  int   keyword_count;
};

// prototypes for functions, full documentation with definitions below
int djb2(const char *key);
void reset_order(const char* key, int* order);
void free_value(const char *key, int *value);

/**
 * Creates a cooccurrence matrix that counts cooccurrences of the given keywords
 * and is initialized to 0 for all entries.
 *
 * @param key an array of distinct non-NULL strings, non-NULL
 * @param n the size of that array
 * @return a pointer to a new cooccurrence matrix; the caller is responsible for
 * destroying the matrix
 */
cooccurrence_matrix *cooccur_create(char *key[], int n)
{
    // malloc space for a new coccurence matrix
    cooccurrence_matrix* result = malloc(sizeof(cooccurrence_matrix) * 1);

    // create necessary smaps
    result->keyword_vectors = smap_create(djb2);
    result->keyword_orders = smap_create(djb2);
    for(int i = 0; i < n; i++)
    {
        // create enough vectors as there are keywords
        int* anonymous_vector = malloc(sizeof(int) * n);
        memset(anonymous_vector, 0, sizeof(int) * n);
        smap_put(result->keyword_vectors, key[i], anonymous_vector);

        // create enough ordering values as there are keywords
        int* order = malloc(sizeof(int) * 2);
        order[0] = 0; // dirty trick to make duplicate checking faster
        order[1] = i; // universal index for this keyword in ALL vectors
        smap_put(result->keyword_orders, key[i], order);
    }

    // update metadata
    result->keyword_count = n;

    return result;
}

/**
 * Updates the given cooccurrence matrix by incrementing the counts for
 * each pair of keywords in the given context.
 *
 * @param mat a pointer to a cooccurrence matrix, non-NULL
 * @param context an array of distinct non-NULL strings that are keywords
 * for that matrix, non-NULL
 * @param n the size of that array
 */
void cooccur_update(cooccurrence_matrix *mat, char **context, int n)
{
    smap* keyword_vectors = mat->keyword_vectors;
    smap* keyword_orders = mat->keyword_orders;

    // for each keyword in the context
    for(int i = 0; i < n; i++)
    {
        // take its vector
        int* int_vector = smap_get(keyword_vectors, context[i]);

        // and then for each keyword in the context again
        for(int j = 0; j < n; j++)
        {
            // find what position it is in, in the vector
            int* order = smap_get(keyword_orders, context[j]);
            int index = order[1];

            // and increment its count
            int_vector[index] = int_vector[index] + 1;
        }
    }
}

/**
 * Reads keywords from the given matrix from the current line of the given stream.
 * A keyword is considered to have been read if it appears on the stream before
 * the next newline surrounded by whitespace, the beginning of input (as seen by
 * this function), or EOF.  The newline is read and removed from the stream.
 *
 * @param mat a pointer to a cooccurrence matrix, non-NULL
 * @param stream a stream, non-NULL
 * @param n a pointer to an int where the size of the returned array will be written
 * @return an array of unique non-NULL strings containing all the keywords read;
 * the caller is responsible for deallocating the array and the strings it contains
 *
 * cooccur_read_context: O(m) where m is the number of words in the context
 */
char **cooccur_read_context(cooccurrence_matrix *mat, FILE *stream, int *n)
{
    // the magic starts here
    smap* keyword_orders = mat->keyword_orders;

    // read the line
    size_t len = 0;
    char* line = NULL;
    size_t result = getline(&line, &len, stream);

    if(result != -1)
    {
        // get length of string
        len = strlen(line);

        // remove the \n char at the end
        if(line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        // a new array of strings as big as the tokens
        int stripped_size = 0;
        char** stripped = malloc(sizeof(char*) * len);

        // tokenize by space
        char* line_alt = line;
        char* tok_ptr = strtok(line_alt, " ");
        while(tok_ptr != NULL)
        {
            bool check = smap_contains_key(keyword_orders, tok_ptr);
            if(check)
            {
                int* order = smap_get(keyword_orders, tok_ptr);
                if(order[0] == 0)
                {
                    stripped[stripped_size] = strdup(tok_ptr);
                    stripped_size = stripped_size + 1;
                    order[0] = order[0] + 1;
                }
            }

            // get the next token
            tok_ptr = strtok(NULL, " ");
        }

        // free the line
        free(line);

        // set the rest of stripped to NULL
        /*for(int k = stripped_size; k < len; k++)
        {
            stripped[k] = NULL;
        }*/

        // if no words have been added, just return NULL
        if(stripped_size == 0)
        {
            *n = 0;
            free(stripped);
            return NULL;
        }

        // truncate the size of stripped if needed
        stripped = realloc(stripped, sizeof(char*) * stripped_size);
        if(stripped == NULL)
        {
            free(stripped);
            *n = -1;
            return NULL;
        }

        // update the size
        *n = stripped_size;

        // reset orders for next line
        smap_for_each(keyword_orders, reset_order);

        // remember to free stripped in main!
        return stripped;
    }
    else
    {
        // if getline did not work
        free(line);
        *n = -1;
        return NULL;
    }
}

/**
 * Returns the vector (row) for the given word in the given matrix.
 * Values in the returned array correspond to the keywords for the
 * given matrix in the order they were given in the array passed to
 * the call to cooccur_create that created the given matrix.  Each
 * value is the proportion of times that the given keyword appeared in
 * a call to cooccur_update for the given matrix that also contained
 * each other keyword.  If the given word is not a keyword for the
 * given matrix or the keyword has never appeared in a call to
 * cooccur_update for the given matrix, then the returned array
 * contains 0.0 in every entry.
 *
 * @param mat a pointer to a cooccurrence matrix, non-NULL
 * @param word a string, non-NULL
 * @return an array of doubles; it is the caller's responsibility to deallocate that array
 */
double *cooccur_get_vector(cooccurrence_matrix *mat, const char *word)
{
    smap* keyword_vectors = mat->keyword_vectors;
    smap* keyword_orders = mat->keyword_orders;
    int keyword_count = mat->keyword_count;

    // get_vector will irreversibly divide the values as well
    int* order = smap_get(keyword_orders, word);
    int index = order[1];

    // we will copy values into an array of doubles
    double* double_vector = malloc(sizeof(double) * keyword_count);

    // first see if the key even exists
    bool result = smap_contains_key(keyword_vectors, word);
    if(result)
    {
        // get the int vector
        int* int_vector = smap_get(keyword_vectors, word);

        // also find the word's own count
        int self_count = int_vector[index];

        // now for each keyword in the vector
        for(int i = 0; i < keyword_count; i++)
        {
            // divide the int by the self_count and copy it as a double
            if(self_count != 0)
            {
                double_vector[i] = (double) int_vector[i] / self_count;
            }
            else
            {
                double_vector[i] = (double) int_vector[i];
            }
        }

        return double_vector;
    }
    else
    {
        return NULL;
    }
}

/**
 * Destroys the given matrix.
 *
 * @param mat a pointer to a cooccurrence matrix, non-NULL
 */
void cooccur_destroy(cooccurrence_matrix *mat)
{
    smap_for_each(mat->keyword_vectors, free_value);
    smap_for_each(mat->keyword_orders, free_value);
    smap_destroy(mat->keyword_vectors);
    smap_destroy(mat->keyword_orders);
    free(mat);
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

/**
 * an smap_for_each helper
 * @param key   unused
 * @param order the order/value associated with the key
 */
void reset_order(const char* key, int* order)
{
    order[0] = 0;
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
