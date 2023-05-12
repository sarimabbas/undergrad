#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "plist.h"

struct plist
{
    int size; // the size of the plist i.e. how many points there are inside
    int capacity; // the capacity of the plist i.e. how many malloc blocks are available
    point* point_data; // a pointer to the first point and also the malloc block of points as a whole
};

#define PLIST_INITIAL_SIZE 2

// i'm adding this function because it was useful for todo_list and should be useful for this!
void plist_embiggen(plist *l);

plist *plist_create()
{
    plist *l = malloc(sizeof(plist));

    // check for successful malloc
    if(l != NULL)
    {
        // set size parameter to 0
        l->size = 0;
        // malloc 2 blocks for the points
        l->point_data = malloc(sizeof(point) * PLIST_INITIAL_SIZE);
        // check for successful pointer creation
        l->capacity = (l->point_data != NULL ? PLIST_INITIAL_SIZE : 0);
    }

    return l;
}

void plist_destroy(plist *l)
{
    // first free the malloc blocks that pertain to point data
    free(l->point_data);
    // then free the plist struct itself
    free(l);
}

int plist_size(const plist *l)
{
    // return the size parameter from the plist struct
    return l->size;
}

// don't worry, add_end will add to existing empty malloc blocks too
bool plist_add_end(plist *l, const point *p)
{
    if(l->size == l->capacity)
    {
        plist_embiggen(l); // #IMPROVE : return an error code for embiggen for the return
    }

    if(l->size < l->capacity)
    {
        // access the point data field. index into it like an array with the size parameter, and copy
        // the point just by dereferencing it
        l->point_data[l->size] = *p;
        l->size++;
    }

    return true;
}

void plist_embiggen(plist *l)
{
    // create an increased capacity
    int larger_cap = (l->capacity * 2 > PLIST_INITIAL_SIZE ? l->capacity * 2 : PLIST_INITIAL_SIZE);

    // create a new pointer to reallocated point data
    // (this also frees the old point_data pointer)
    point* larger = realloc(l->point_data, larger_cap * sizeof(point));

    // if the reallocation is successful
    if(larger != NULL)
    {
        // assign the original pointer to the new
        l->point_data = larger;
        // update the capacity parameter
        l->capacity = larger_cap;
    }
}

/**
 * Copies the point at the given location in the given list into the
 * given point.  There is no effect if the index is not valid.
 *
 * @param l a pointer to a list, non-NULL
 * @param i an index into l
 * @param p a pointer to a point, non-NULL
 */
void plist_get(const plist *l, int i, point *p)
{
    if(i < l->capacity)
    {
        *p = l->point_data[i];
    }
}

/**
 * Determines if the given point is contained in the given list.
 *
 * @param l a pointer to a list, non-NULL
 * @param p a pointer to a point, non-NULL
 * @return true if and only if l contains a point with the same coordinates as p
 */
bool plist_contains(const plist *l, const point *p)
{
    double x_coord_fixed = p->x;
    double y_coord_fixed = p->y;

    if(l->size == 0)
    {
        return false;
    }

    for(int i = 0; i < l->size; i++)
    {
        double x_coord_current = l->point_data[i].x;
        double y_coord_current = l->point_data[i].y;

        if(x_coord_fixed == x_coord_current && y_coord_fixed == y_coord_current)
        {
            return true;
        }
    }
    return false;
}

/**
 * Prints the given list to the given stream using the given format for
 * each point.  If there is a newline at the end of the format specifier
 * then that newline is printed once at the end of the list.
 *
 * @param stream a pointer to astream, non-NULL
 * @param fmt a format specifier string, non-NULL
 * @param l a pointer to a list, non-NULL
 */
void plist_fprintf(FILE *stream, const char *fmt, const plist *l)
{
    // start by making a copy of the format specifier
    char fmt_cpy[50];
    strcpy(fmt_cpy, fmt);

    // check if \n exists, and remove it if it does
    bool newline_exists = false;
    if(strstr(fmt_cpy, "\n") != NULL)
    {
        newline_exists = true;
        // remove the format specifier
        fmt_cpy[strcspn(fmt_cpy, "\n")] = 0;
    }

    // get size of the list
    int size = plist_size(l);

    // begin printing
    fprintf(stream, "[");
    for(int i = 0; i < size; i++)
    {
        if(i == size - 1)
        {
            point_fprintf(stream, fmt_cpy, &l->point_data[i]);
            break;
        }
        else
        {
            point_fprintf(stream, fmt_cpy, &l->point_data[i]);
            fprintf(stream, ", ");
        }
    }
    if(newline_exists) { fprintf(stream, "]\n"); } else { fprintf(stream, "]"); }
}
/**
 * Sorts the given list using the given comparator.
 *
 * @param l a pointer to a list
 * @param compare a pointer to a function that returns a negative
 * number to indicate the first point comes before the second, a
 * positive number to indicate the second point comes before the
 * first, and zero if the have the same ordinal value.
 */
void plist_sort(plist *l, int (*compare)(const point*, const point*))
{
    qsort(l->point_data, l->size, sizeof(point), (int (*) (const void*, const void*))compare);
}
