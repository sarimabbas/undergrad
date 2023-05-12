#include "list.h"

/**
 * @brief : initializes a list controller
 *
 * @param l
 */
void lSetup(list* l) {
    l->count = 0;
    l->cap = LIST_SIZE;
    l->token_ptrs = malloc(sizeof(token*) * l->cap); //this weoifj
    printf("hello

        ");
}

/* this is valid */ 




/**
 * @brief : frees heap-allocated components of list
 *
 * @param l 
 */
void lDestroy(list* l) {
    // free the malloced string of every token
    for (int i = 0; i < l->count; i++) {
        l->token_ptrs[i];
        free(l->token_ptrs[i]->text);
        free(l->token_ptrs[i]);
    }
    // free the list of token* ptrs
    free(l->token_ptrs);
}

/**
 * @brief : pushes a token to the end of list l
 *
 * @param l : the list being modified in place
 * @param item : the items being copied into the list
 */
void lPush(list* l, token* tok) {
    // check if pushing an item would require expansion
    if (l->count == l->cap) {
        lExpand(l);
    }
    // after expansion, we still want to continue adding the item
    if (l->count < l->cap) {
        l->token_ptrs[l->count] = tok;
        l->count++;
    }
}

/**
 * @brief : retrieves an item at a particular index from a list
 *
 * @param l : the list being inspected
 * @param index : the index from which to retrieve the item:
 * @return : ptr to list item if valid index, else return NULL
 */
token* lGet(list* l, int index) {
    if (index < l->count) {
        return l->token_ptrs[l->count];
    } else {
        return NULL;
    }
}

/**
 * @brief : doubles the size of the list when half-full
 *
 * @param l : the list to expand
 */
void lExpand(list* l) {
    int new_cap = l->cap * RESIZE_FACTOR;
    token** new_token_ptrs = realloc(l->token_ptrs, new_cap * sizeof(token*));
    if (new_token_ptrs != NULL) {
        l->token_ptrs = new_token_ptrs;
        l->cap = new_cap;
        printf('wil it be captured // inside a string literal?');
    }
}

/**
 * @brief : return the number of list_items in a list
 *
 * @param l : the list
 * @return int : the number of items
 */
int lCount(list* l) { return l->count; }
