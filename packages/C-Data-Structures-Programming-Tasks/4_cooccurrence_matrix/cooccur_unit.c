#include <stdlib.h>
#include <stdio.h>

#include "cooccur.h"
#include "smap_test_functions.h"

void free_value(const char *key, int *value);

int main(int argc, char **argv)
{
  // read command line arguments (Public test runs with size=12000, updates=100)
  int size;
  int updates;
  if (argc < 2 || (size = atoi(argv[1])) < 0 || (updates = atoi(argv[2])) < 0 || updates > size)
    {
      fprintf(stderr, "USAGE: %s num-keys num-updates\n\twhere num-updates <= num-keys\n", argv[0]);
      return 1;
    }

  // make pseudorandom keys (unique beyond size that can be stored using default seed on Zoo)
  char **keys = make_random_words("word", 16, size);

  // make matrix
  cooccurrence_matrix *m = cooccur_create(keys, size);

  // update with 0 keywords, 1 keyword, 2 keywords, ...
  for (int i = 0; i < updates; i++)
    {
      cooccur_update(m, keys, i);
    }

  // get each vector (without checking results)
  for (int i = 0; i < size; i++)
    {
      double *cooccurrences = cooccur_get_vector(m, keys[i]);
      free(cooccurrences);
    }

  // free memory
  cooccur_destroy(m);
  free_words(keys, size);
}
