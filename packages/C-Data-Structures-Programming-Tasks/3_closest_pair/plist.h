#ifndef __PLIST_H__
#define __PLIST_H__

#include <stdio.h>
#include <stdbool.h>

#include "point.h"

struct plist;
typedef struct plist plist;

/**
 * Creates an empty list of points.
 *
 * @return a pointer to the list, or NULL if allocation was unsuccessful
 */
plist *plist_create();

/**
 * Destroys the given list.  The list is invalid after it has been destroyed.
 *
 * @param l a pointer to a list, non-NULL
 */
void plist_destroy(plist *l);

/**
 * Returns the number of points on the given list.
 *
 * @param l a pointer to a list, non-NULL
 * @return the number of points in l
 */
int plist_size(const plist *l);

/**
 * Add a copy of the given point to the end of the given list.
 *
 * @param l a pointer to a list, non-NULL
 * @param p a pointer to a point, non-NULL
 * @return true if and only if the point was successfully added
 */
bool plist_add_end(plist *l, const point *p);

/**
 * Copies the point at the given location in the given list into the
 * given point.  There is no effect if the index is not valid.
 *
 * @param l a pointer to a list, non-NULL
 * @param i an index into l
 * @param p a pointer to a point, non-NULL
 */
void plist_get(const plist *l, int i, point *p);

/**
 * Determines if the given point is contained in the given list.
 *
 * @param l a pointer to a list, non-NULL
 * @param p a pointer to a point, non-NULL
 * @return true if and only if l contains a point with the same coordinates as p
 */
bool plist_contains(const plist *l, const point *p);

/**
 * Prints the given list to the given stream using the given format for
 * each point.  If there is a newline at the end of the format specifier
 * then that newline is printed only once at the end of the list.  The
 * contents of the list are enclosed in [] and each item is separated by
 * a comma and a single space.
 *
 * @param stream a pointer to astream, non-NULL
 * @param fmt a format specifier string, non-NULL
 * @param l a pointer to a list, non-NULL
 */
void plist_fprintf(FILE *stream, const char *fmt, const plist *l);

/**
 * Sorts the given list using the given comparator.
 *
 * @param l a pointer to a list
 * @param compare a pointer to a function that returns a negative
 * number to indicate the first point comes before the second, a
 * positive number to indicate the second point comes before the
 * first, and zero if the have the same ordinal value.
 */
void plist_sort(plist *l, int (*compare)(const point*, const point*));

#endif
