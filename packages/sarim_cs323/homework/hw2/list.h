#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parsnip.h"

#define LIST_SIZE 10
#define RESIZE_FACTOR 2

typedef struct list
{
    int count;              // no of items added to list
    int cap;                // the capacity of the list
    token** token_ptrs;     // list of tokens (heap allocated)

} list;

/**
 * @brief : initializes a stack-allocated tokList in place
 *
 * @param l
 */
void lSetup(list* l);

/**
 * @brief : frees heap-allocated components of list
 *
 * @param l
 */
void lDestroy(list* l);

/**
 * @brief : pushes a token to the end of list l
 *
 * @param l : the list being modified in place
 * @param item : the items being copied into the list
 */
void lPush(list* l, token* tok);

/**
 * @brief : retrieves an item at a particular index from a list
 *
 * @param l : the list being inspected
 * @param index : the index from which to retrieve the item:
 * @return : ptr to list item if valid index, else return NULL
 */
token* lGet(list* l, int index);

/**
 * @brief : doubles the size of the list when half-full
 *
 * @param l : the list to expand
 */
void lExpand(list* l);

/**
 * @brief : return the number of list_items in a list
 *
 * @param l : the list
 * @return int : the number of items
 */
int lCount(list* l);
