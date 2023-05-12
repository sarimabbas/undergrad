#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Compares the two given strings based on their distance to "BWI".
 *
 * @param a a pointer to a string, non-NULL
 * @param b a pointer to a string, non-NULL
 * @return a negative number if a is closer to "BWI" than b, zero
 * if they are the same distance, and a positive bumber if a is further than b,
 * where distance is determined by the char_distance function.
 */
int compare_airports(const void *a, const void *b);

/**
 * Returns the distance between the two strings, where "distance" means
 * the sum of the absolute value of the difference on the corresponding
 * characters, up to the last character in the shorter string.
 *
 * @param s1 a string, non-NULL
 * @param s2 a string, non-NULL
 * @return the distance between the strings
 */
int char_distance(const char *s1, const char *s2);

/**
 * Calls strcmp on the strings pointed to by the given pointers
 * and returns the result.
 *
 * @param s1 a pointer to a string, non-NULL
 * @param s2 a pointer to a string, non-NULL
 * @return negative if the 1st string pointed to is smaller, 0 if same,
 * positive if greater
 */
int normal_string_compare(const void *s1, const void *s2);

int main(int argc, char **argv)
{
  // copy command-line arguments
  char *input_dist[argc - 1];
  char *input_alpha[argc - 1];

  for (int i = 1; i < argc; i++)
    {
      input_dist[i - 1] = strdup(argv[i]); // strdup just for kicks
      input_alpha[i - 1] = input_dist[i - 1]; // strdup just for kicks
    }

  // sort by normal string sort and by distance from "BWI"
  qsort(input_alpha, argc - 1, sizeof(char *), normal_string_compare);
  qsort(input_dist, argc - 1, sizeof(char *), compare_airports);

  // output sorted arrays
  for (int i = 0; i < argc - 1; i++)
    {
      printf("%s %s\n", input_alpha[i], input_dist[i]);
    }

    for (int i = 0; i < argc - 1; i++)
    {
      free(input_dist[i]);
    }
}

int compare_airports(const void *x, const void *y)
{
  char *s1 = *(char **)x;
  char *s2 = *(char **)y;

  return char_distance(s1, "BWI") - char_distance(s2, "BWI");
}

int char_distance(const char *s1, const char *s2)
{
  int sum = 0;
  while (*s1 != '\0' && *s2 != '\0')
    {
      sum += abs(*s1 - *s2);
      s1++;
      s2++;
    }
  return sum;
}

int normal_string_compare(const void *s1, const void *s2)
{
  const char * const *a = s1;
  const char * const *b = s2;

  return strcmp(*a, *b);
}

