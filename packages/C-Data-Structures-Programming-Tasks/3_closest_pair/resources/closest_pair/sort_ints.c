#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Compares the ints (given as pointers to them) and returns whether 
 * the first is smaller than , equal to, or greater than the second.
 *
 * @param s1 a pointer to an int, non-NULL
 * @param s2 a pointer to an int, non-NULL
 * @return a negative number of the first int is smaller than the second,
 * zero if they are equal, and a positive number if the first is larger
 */
int compare_ints(const void *s1, const void *s2);

/**
 * Sorts the given array using the given comparison function.
 *
 * This function is polymorphic -- it will work with any type.
 * Because it works with any type, it accepts the array as a void *
 * so we can pass any pointer to it whether it is a pointer to the
 * first element of an array of ints, the pointer to the first element
 * of an array of struct points, or whatever.  Like most functions that
 * work with arrays, it needs to know how many elements are in the array.
 * In order to do the pointer arithmetic, it needs to know how many bytes
 * each element occupies.
 *
 * @param array a pointer
 * @param item_count a positive integer
 * @param item_size a positive integer
 * @param compare a pointer to a comparison function
 */
void bsort(void *array, int item_count, int item_size, int (*compare)(const void *, const void *));

int main(int argc, char **argv)
{
  // copy command line arguments into an array, converting to ints as we go
  int input_b[argc - 1];
  int input_q[argc - 1];

  for (int i = 1; i < argc; i++)
    {
      input_b[i - 1] = atoi(argv[i]);
      input_q[i - 1] = input_b[i - 1];
    }

  // sort arguments using our slow sort and stdlib's quicker sort
  bsort(input_b, argc - 1, sizeof(int), compare_ints);
  qsort(input_q, argc - 1, sizeof(int), compare_ints);

  // output contents of sorted array
  for (int i = 0; i < argc - 1; i++)
    {
      printf("%d %d\n", input_b[i], input_q[i]);
    }
}

void bsort(void *a, int item_count, int item_size, int (*compare)(const void *, const void *))
{
  char *array = a;
  void *temp = malloc(item_size);

  for (int i = 0; i < item_count - 1; i++)
    {
      for (int j = 0; j < item_count - 1 - i; j++)
	{
	  if (compare(array + j * item_size, array + (j + 1) * item_size) > 0)
	    {
	      // swap elements at indices j and j + 1
	      memcpy(temp, array + j * item_size, item_size);
	      memcpy(array + j * item_size, array + (j + 1) * item_size, item_size);
	      memcpy(array + (j + 1) * item_size, temp, item_size);
	    }
	}
    }
  
  free(temp);
}

	   
int compare_ints(const void *s1, const void *s2)
{
  const int *i1 = s1;
  const int *i2 = s2;

  return *i1 - *i2;
}

