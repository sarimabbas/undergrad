#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "point.h"
#include "plist.h"

// The functions below are *suggestions* and as such may be incompletely
// or overly specified.  Those functions that have the skeleton of a body
// provided may be incomplete with sections that you must add.

/**
 * Compares the two given points based on x-coordinate.
 *
 * @param p1 a pointer to a point, non-NULL
 * @param p2 a pointer to a point, non-NULL
 * @return a negative number if p1 comes before p2, positive if p1 comes
 * after p2, and 0 if they are the same
 */
int point_compare_x(const point *p1, const point *p2)
{
	if(p1->x < p2->x)
	{
		return -1;
	}
	else if(p1->x > p2->x)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * Compares the two given points based on y-coordinate.
 *
 * @param p1 a pointer to a point, non-NULL
 * @param p2 a pointer to a point, non-NULL
 * @return a negative number if p1 comes before p2, positive if p1 comes
 * after p2, and 0 if they are the same
 */
int point_compare_y(const point *p1, const point *p2)
{
	if(p1->y < p2->y)
	{
		return -1;
	}
	else if(p1->y > p2->y)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * Reads point from the given stream and adds them to the given list.
 *
 * @param stream a pointer to an input stream
 * @param l a pointer to a list
 * @param n a non-negative integer
 */
void read_points(FILE *stream, plist *l, int n)
{
	// create a point
	point new_point;
	for(int i = 0; i < n; i++)
	{
		// populate the two coordinates for the point
		fscanf(stream, " %lf %lf", &new_point.x, &new_point.y);

		// add to plist
		plist_add_end(l, &new_point);
	}
}

/**
 * Copies the points from the source list to the destination list
 * in the order they appear in the source list.
 *
 * @param dest a pointer to a list, non-NULL
 * @param source a pointer to a list, non-NULL
 */
void copy_list(plist *dest, const plist* source)
{
	int size = plist_size(source);
	point new_point;
	for(int i = 0; i < size; i++)
	{
		// assuming the destination list is empty to begin with
		// you can start adding some points
		// don't worry, add_end will add to existing empty malloc blocks too
		plist_get(source, i, &new_point);
		plist_add_end(dest, &new_point);
	}
}

/**
 * Returns the closest pair of points among those on the given list.
 * The closest pair are returned in p1 and p2 and their distance is
 * returned in d.  This implementation uses the O(n log n) divide-and-conquer
 * algorithm.
 *
 * @param l a pointer to a list of at least 2 points
 * @param p1 a pointer to a point, non-NULL
 * @param p2 a pointer to a point, non-NULL
 * @param d a pointer to a double, non-NULL
 */
void closest_pair(const plist *list_x, const plist *list_y, point *p1, point *p2, double *d);

/**
 * Returns the closest pair of points among those on the given list.
 * The closest pair are returned in p1 and p2 and their distance is
 * returned in d.  This implementation uses the Theta(n^2) brute force
 * algorithm.
 *
 * @param l a pointer to a list of at least 2 points
 * @param p1 a pointer to a point, non-NULL
 * @param p2 a pointer to a point, non-NULL
 * @param d a pointer to a double, non-NULL
 */
void closest_pair_brute_force(const plist *l, point *p1, point *p2, double *d)
{
	*d = -1;
	int size = plist_size(l);

	if(size == 0 || size == 1)
	{
		*d = -1;
		printf("error, closest_pair_brute_force expecting atleast 2 points, got %i\n", size);
		return;
	}

	if(size == 2)
	{
		plist_get(l, 0, p1);
		plist_get(l, 1, p2);
		*d = point_distance(p1, p2);
		return;
	}

	point base_point;
	point compare_point;
	for(int i = 0; i < size; i++)
	{
		// populate the base point with the index
		plist_get(l, i, &base_point);
		for(int j = i + 1; j < size - 1; j++)
		{
			// populate the compare point with the index
			plist_get(l, j, &compare_point);

			// make the calculation between the points
			double calculation = point_distance(&base_point, &compare_point);

			// use the distance to compare with the shortest distance
			if(*d < 0 || calculation < *d)
			{
				*d = calculation; // update shortest distance
				plist_get(l, i, p1); // update point p1
				plist_get(l, j, p2); // update point p2
			}
		}
	}
}

/**
 * Splits the given list by adding the leftmost half of the points
 * in order of increasing x-coordinate to the end of left and the
 * rightmost half of the points in order of increasing x-coordinate
 * to to the end of right.
 *
 * @param l a pointer to a list sorted by x-coordinate, non-NULL
 * @param left a pointer to a list, non-NULL
 * @param right a pointer to a list, non-NULL
 */
void split_list_x(const plist *l, plist *left, plist *right)
{
	int size = plist_size(l);
	int left_size = size / 2;
	point new_point;

	for(int i = 0; i < left_size; i++)
	{
		plist_get(l, i, &new_point);
		plist_add_end(left, &new_point);
	}

	for(int i = left_size; i < size; i++)
	{
		plist_get(l, i, &new_point);
		plist_add_end(right, &new_point);
	}
}

/**
 * Splits the given list by adding the leftmost half of the points
 * in order of increasing y-coordinate to the end of y_left and the
 * rightmost half of the points in order of increasing y-coordinate
 * to to the end of y_right.
 *
 * @param l a pointer to a list sorted by y-coordinate, non-NULL
 * @param x_left a pointer to a list containing the leftmost half of the points
 * on l in order of increasing x-coordinate
 * @param x_right a pointer to a list containing the rightmost half of the
 * points on l in order of increasing x-coordinate
 * @param y_left a pointer to a list
 * @param y_right a pointer to a list
 *
 * @param end a pointer to a list, non-NULL
 */

void split_list_y(const plist *l, const plist *x_left, const plist *x_right,
		  plist *y_left, plist *y_right)
{
	// find the sizes of the main list and x_left
	int l_size = plist_size(l);
	int x_left_size = plist_size(x_left);
	int x_right_size = plist_size(x_right);

	// use this information to access the extreme points
	// x_left = [{0, 1}, {1,3}]
	// x_right = [{3, 4}, {5,6}]
	point x_left_right_extreme;
	point x_right_left_extreme;
	plist_get(x_left, x_left_size - 1, &x_left_right_extreme); // {1, 3}
	plist_get(x_right, 0, &x_right_left_extreme); // {3, 4}

	// find the dividing line between x_left and x_right
	double checker;
	if(x_left_size == x_right_size)
	{
		double midpoint = (x_left_right_extreme.x + x_right_left_extreme.x) / 2;
		checker = midpoint;
	}
	else if(x_left_size > x_right_size)
	{
		checker = x_left_right_extreme.x;
	}
	else
	{
		checker = x_right_left_extreme.x;
	}

	// use this information to organise the points into either y_left or y_right
	point test;
	for(int i = 0; i < l_size; i++)
	{
		plist_get(l, i, &test);
		if(test.x < checker)
		{
			plist_add_end(y_left, &test);
		}
		else
		{
			plist_add_end(y_right, &test);
		}
	}
}

/**
 * Adds the points in the first given list with x-coordinates in the given
 * range to the second list in order of increasing y-coordinate
 *
 * @param list_y a pointer to a list of points sorted by y-coordinate, non-NULL
 * @param middle a pointer to a list, non-NULL
 * @param left a real number
 * @param right a real number greater than or equal to left
 */
void make_middle(const plist *list_y, plist *middle, double left, double right)
{
	int size = plist_size(list_y);
	point test;
	for(int i = 0; i < size; i++)
	{
		// copy into the point from list_y
		plist_get(list_y, i, &test);

		// if test point falls within range
		if(left <= test.x && test.x <= right)
		{
			plist_add_end(middle, &test);
		}
	}
}

/**
 * Searches the given list for a pair of points closer than d units.
 * If such a pair is found, p1 and p2 are updated to the closest such pair
 * and d is updated to their distance.
 *
 * @param middle a pointer to a list of points sorted by y-coordinate, non-NULL
 * @param p1 a pointer to a point, non-NULL
 * @param p2 a pointer to a point, non-NULL
 * @param d a pointer to a positive real number, non-NULL
 */
void search_middle(const plist *middle, point *p1, point *p2, double *d)
{
	int size = plist_size(middle);
	double min = *d;  // Initialize the minimum distance as d
    // This loop runs at most 6 times
    for(int i = 0; i < size; i++)
	{
		point i_point;
		plist_get(middle, i, &i_point);

		int j_mirror = i + 1;
		point j_point;
		plist_get(middle, j_mirror, &j_point);

		for(int j = i + 1; j < size && (j_point.y - i_point.y) < min; j++)
		{
			point i_point_inner;
			point j_point_inner;
			plist_get(middle, i, &i_point_inner);
			plist_get(middle, j, &j_point_inner);

			if(point_distance(&i_point_inner, &j_point_inner) < min)
			{
				min = point_distance(&i_point_inner, &j_point_inner);
				plist_get(middle, i, p1);
				plist_get(middle, j, p2);
			}
		}
	}
	*d = min;
    return;
}

int main(int argc, char **argv)
{
	// create empty lists
	plist *list_x = plist_create();
	plist *list_y = plist_create();

	if (list_x == NULL || list_y == NULL)
	{
		if (list_x != NULL)
		{
		plist_destroy(list_x);
		}

		if (list_y != NULL)
		{
		plist_destroy(list_y);
		}

		printf("%s: could not allocate lists\n", argv[0]);
		return 1;
	}

	// read n
	int n;
	fscanf(stdin, "%d", &n);

	// read into one list
	read_points(stdin, list_x, n);

	// sort list
	plist_sort(list_x, point_compare_x);

	// end if two points or less given
	if(n == 2)
	{
		point p1;
		point p2;
		plist_get(list_x, 0, &p1);
		plist_get(list_x, 1, &p2);
		printf("(%f, %f)-(%f, %f)=%f\n", p1.x, p1.y, p2.x, p2.y, point_distance(&p1, &p2));
		return 0;
	}
	else if(n < 2)
	{
		printf("error: ClosestPair requires atleast 2 points\n");
		return 1;
	}

	// check for distinctness
	// within sorted array this should take about O(n) time
	int list_x_size = plist_size(list_x);
	double zero = 0;
	point first;
	point second;
	for(int i = 1; i < list_x_size; i++)
	{
		plist_get(list_x, i - 1, &first);
		plist_get(list_x, i, &second);
		if(first.x == second.x && first.y == second.y)
		{
			printf("(%f, %f)-(%f, %f)=%f\n", first.x, first.y, second.x, second.y, zero);
			return 0;
		}
	}

	// make list_y a copy of list_x
	copy_list(list_y, list_x);

	if (plist_size(list_y) == n)
	{
		// sort the y-list
		plist_sort(list_y, point_compare_y);

		point p1, p2;
		double d;
		closest_pair(list_x, list_y, &p1, &p2, &d);
		printf("(%f, %f)-(%f, %f)=%f\n", p1.x, p1.y, p2.x, p2.y, d);
	}
	else
	{
		// memory allocation must have failed when copying points
		fprintf(stdout, "%s: failed to read points\n", argv[0]);
	}

  plist_destroy(list_x);
  plist_destroy(list_y);
}

void closest_pair(const plist *list_x, const plist *list_y, point *p1, point *p2, double *d)
{
	int n = plist_size(list_x);

	if (n <= 3)
	{
		closest_pair_brute_force(list_x, p1, p2, d);
		return;
	}

	// make left/right lists
	plist *x_left, *x_right, *y_left, *y_right;
	x_left = plist_create();
	x_right = plist_create();
	y_left = plist_create();
	y_right = plist_create();

	// populate left/right lists
	split_list_x(list_x, x_left, x_right);
	split_list_y(list_y, x_left, x_right, y_left, y_right);

	// recursively find closest pair in two halves and keep the closer of those
	point p1_left, p2_left;
	double d_left;
	closest_pair(x_left, y_left, &p1_left, &p2_left, &d_left);

	point p1_right, p2_right;
	double d_right;
	closest_pair(x_right, y_right, &p1_right, &p2_right, &d_right);

	// determine which pair is closer together
	if(d_left <= d_right) // doing less than equals to handle strange cases
	{
		*d = d_left;
		*p1 = p1_left;
		*p2 = p2_left;
	}
	else
	{
		*d = d_right;
		*p1 = p1_right;
		*p2 = p2_right;
	}

	// create the list of points in the middle
	double mid;
	plist *middle = plist_create();

	point x_left_right_point; // the right-most point in x_left
	plist_get(x_left, plist_size(x_left) - 1, &x_left_right_point);
	point x_right_left_point; // the left-most point in x_right
	plist_get(x_right, 0, &x_right_left_point);

	mid = (x_left_right_point.x + x_right_left_point.x) / 2;

	// populate that list
	make_middle(list_y, middle, mid - *d, mid + *d);

	// search the list of points in middle for a closer pair
	search_middle(middle, p1, p2, d);

	// see which point comes first by x-coordinate
	if(p2->x < p1->x)
	{
		point temp = *p1;
		*p1 = *p2;
		*p2 = temp;
	}

	// clean up
	plist_destroy(x_left);
	plist_destroy(x_right);
	plist_destroy(y_left);
	plist_destroy(y_right);
	plist_destroy(middle);
}
