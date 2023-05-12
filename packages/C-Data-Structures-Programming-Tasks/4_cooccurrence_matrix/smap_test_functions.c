#include <stdlib.h>
#include <string.h>

#include "smap_test_functions.h"

#define MAX_INT_DIGITS 10

char **make_random_words(const char *prefix, int len, int n)
{
  if (prefix == NULL || len < strlen(prefix) || n < 0)
    {
      return NULL;
    }

  const char *alphabet = "ABCDEFGHIJKLMNIPQRSTUVWXYZ";
  int alphabet_len = strlen(alphabet);

  int prefix_len = strlen(prefix);

  char **arr = malloc(sizeof(char *) * n);
  if (arr != NULL)
    {
      for (int i = 0; i < n; i++)
	{
	  arr[i] = malloc(sizeof(char) * (len + 1));

	  if (arr[i] != NULL)
	    {
	      strcpy(arr[i], prefix);
	      for (int j = prefix_len; j < len; j++)
		{
		  arr[i][j] = alphabet[rand() % alphabet_len];
		}
	      arr[i][len] = '\0';
	    }
	}
    }

  return arr;
}

void free_words(char **arr, int n)
{
  for (int i = 0; i < n; i++)
    {
      free(arr[i]);
    }
  free(arr);
}

int java_hash_string(const char *s)
{
  // from Java's String.hashCode()
  // https://docs.oracle.com/javase/7/docs/api/java/lang/String.html#hashCode()
  if (s == NULL)
    {
      return 0;
    }

  int power = 31;
  int hash = 0;
  const char *p = s + strlen(s) - 1;
  while (p >= s)
    {
      hash += power * *p;
      power *= 31;
      p--;
    }

  if (hash < 0)
    {
      hash = -hash;
      if (hash < 0)
	{
	  hash = 0;
	}
    }

  return hash;
}
