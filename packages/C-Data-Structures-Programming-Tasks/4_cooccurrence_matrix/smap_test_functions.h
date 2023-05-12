#ifndef __SMAP_TEST_FXNS__
#define __SMAP_TEST_FXNS__

#include <stdio.h>

/**
 * Returns an array of randomly generated strings.  Each string starts with the
 * given prefix and contains randomly selected letters from A to Z to fill the
 * strings to the given length.  The strings will be unique with high probability
 * for large enough len compared to n.
 *
 * @param prefix a string, non-NULL
 * @param len an integer at least as large as the length of prefix
 * @param n a non-negative integer
 */
char **make_random_words(const char *prefix, int len, int n);

/**
 * Frees the given array of strings.
 *
 * @param arr an array of strings
 * @param n the length of that array
 */
void free_words(char **arr, int n);

/**
 * Returns a hash value of the given string.
 *
 * @param s a string
 */
int java_hash_string(const char *s);

#endif
