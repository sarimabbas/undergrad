#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "smap.h"

/**
 * An implementation of a map of strings as an unsorted fixed-size array.
 * This implementation will not meet the requirements of Assignment #4 and
 * not completely tested itself but should be sufficient for testing your
 * Cooccurrence matrix.
 */

struct entry
{
  char* key;
  int* value;
  struct entry* next;
};
typedef struct entry entry;

struct smap
{
  int capacity;
  int size;
  int (*hash)(const char *);
  entry** lookup_table; // pointer to an array of pointers to entries
};

#define SMAP_INITIAL_CAPACITY 100

entry* smap_table_find_key(const smap *m, const char *key);
void smap_embiggen(smap *m);

// this bit creates the meta data
// the metadata has the capacity of the array, the initial size 0 and a entry_data pointer
smap *smap_create(int (*h)(const char *s))
{
    smap *result = malloc(sizeof(smap));
    if (result != NULL)
    {
        result->size = 0;
        result->hash = h;

        result->lookup_table = malloc(sizeof(entry*) * SMAP_INITIAL_CAPACITY);

        for(int i = 0; i < SMAP_INITIAL_CAPACITY; i++)
        {
            result->lookup_table[i] = NULL;
        }

        result->capacity = (result->lookup_table != NULL ? SMAP_INITIAL_CAPACITY : 0);
    }
    return result;
}

int smap_size(const smap *m)
{
  return m->size;
}

/**
 * Returns the value of the key in the given map.
 *
 * @param m a map
 * @param key a string, non-NULL
 * @param hash a hash function for strings
 * @returns a pointer to the value of key in the map if found, NULL if not
 */
entry* smap_table_find_key(const smap *m, const char *key)
{
    // immediate search for key within the linked chain to hash
    int key_hash = m->hash(key) % m->capacity;
    entry* current = m->lookup_table[key_hash]; // the pointer to the start of the linked list

    // if there is a linked list starting from that row in the lookup_table
    while(current != NULL)
    {
        if(strcmp(current->key, key) == 0)
        {
            // return a pointer to the entry where the key is found
            return current;
        }
        current = current->next;
    }

    // return NULL if nothing found
    return NULL;
}

bool smap_put(smap *m, const char *key, int *value)
{
    if(m != NULL)
    {
        // conduct a search to see if the key already exists
        entry* search_result = smap_table_find_key(m, key);

        // if it exists, update the value
        if(search_result != NULL)
        {
            *(search_result->value) = *value;
            return true;
        }

        // if it does not exist, compute the location of the key
        int hash_result = m->hash(key) % m->capacity;
        entry* current = m->lookup_table[hash_result];

        // create a new entry
        entry* new = malloc(sizeof(entry) * 1);
        if(new != NULL)
        {
            char* key_copy = malloc(sizeof(strlen(key) + 1));
            strcpy(key_copy, key);

            if(key_copy != NULL)
            {
                new->key = key_copy;
                new->value = value; // copy reference, not data
            }
        }

        if(current == NULL) // if the start of the linked list is empty
        {
            new->next = NULL;
            m->lookup_table[hash_result] = new;
        }
        else // if there is an existing linked list
        {
            new->next = current;
            m->lookup_table[hash_result] = new;
        }

        // update the size of the hash table
        m->size += 1;

        // deal with dangling pointers
        // new = NULL;
        // current = NULL;

        // return successfully
        return true;
    }
    else
    {
        return false; // NULL matrix
    }
}

void smap_embiggen(smap *m)
{
  // TO-DO: 60 write and test embiggen for hash table
}

bool smap_contains_key(const smap *m, const char *key)
{
    if(smap_table_find_key(m, key) != NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int *smap_get(smap *m, const char *key)
{
    entry* result = smap_table_find_key(m, key);
    if(result != NULL)
    {
        return result->value;
    }
    else
    {
        return NULL;
    }
}

void smap_for_each(smap *m, void (*f)(const char *, int *))
{
    // iterate through the capacity of the table
    for(int i = 0; i < m->capacity; i++)
    {
        // create a new pointer to the entry nodes
        entry* current = m->lookup_table[i];

        // while this pointer still points to nodes
        while(current != NULL)
        {
            // apply the function to the entry's key and value
            f(current->key, current->value);
        }
    }
}

void smap_for_each_r(smap *m, void (*f)(const char *, int *, void *), void *arg)
{
    // iterate through the capacity of the table
    for(int i = 0; i < m->capacity; i++)
    {
        // create a new pointer to the entry nodes
        entry* current = m->lookup_table[i];

        // while this pointer still points to nodes
        while(current != NULL)
        {
            // apply the function to the entry's key and value
            f(current->key, current->value, arg);
        }
    }
}

void smap_destroy(smap *m)
{
    // iterate through the capacity of the table
    for(int i = 0; i < m->capacity; i++)
    {
        // create a new pointer to the entry nodes
        entry* current = m->lookup_table[i];

        // while this pointer still points to nodes
        while(current != NULL)
        {
            entry* after = current->next;
            free(current->key); // free the key
            free(current); // free the entry itself
            current = after;
        }
    }

    // free the table
    free(m->lookup_table);

    // free the smap struct
    free(m);
}
