#include <stdlib.h>
#include <stdio.h>

#include "smap.h"
#include "smap_test_functions.h"

void free_value(const char *key, int *value);

int main(int argc, char **argv)
{
  // read command-line arguments (Public test runs with size=1000000)
  int size;
  if (argc < 2 || (size = atoi(argv[1])) < 0)
    {
      fprintf(stderr, "USAGE: %s num-keys\n", argv[0]);
      return 1;
    }

  // create a hash table
  smap *m = smap_create(java_hash_string);

  // create pseudorandom words (unique up to size = 1000000 using the default seed on Zoo)
  char **keys = make_random_words("word", 20, size);

  // add keys to map, each with a unique value
  for (int i = 0; i < size; i++)
    {
      int *value = malloc(sizeof(int));
      *value = size - i;
      smap_put(m, keys[i], value);
    }

  // verify that each key is present and associated with the value we put in
  for (int i = 0; i < size; i++)
    {
      if (smap_contains_key(m, keys[i]))
	{
	  int *value = smap_get(m, keys[i]);
	  if (value == NULL)
	    {
	      printf("Error: value for key %s is null, should be %d\n", keys[i], size - i);
	      return 1;
	    }
	  else if (*value != size - i)
	    {
	      // note that if pseudorandom words are not unique this gives false negative
	      printf("Error: value for key %s is %d, should be %d\n", keys[i], *value, size - i);
	      return 1;
	    }
	}
      else
	{
	  printf("Error: key %s not found\n", keys[i]);
	  return 1;
	}
    }

  // make valgrind (and Prof. Glenn) happy
  smap_for_each(m, free_value);
  smap_destroy(m);
  free_words(keys, size);
}

void free_value(const char *key, int *value)
{
  free(value);
}
