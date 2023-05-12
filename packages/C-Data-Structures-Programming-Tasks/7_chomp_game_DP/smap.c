#define _GNU_SOURCE

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "smap.h"

/**
 * An implementation of a map of strings as an unsorted fixed-size array.
 * This implementation will not meet the requirements of Assignment #4 and
 * not completely tested itself but should be sufficient for testing your
 * Cooccurrence matrix.
 */

typedef struct
{
  char *key;
  int* value;
  bool occupied;
} entry;

struct smap
{
  int capacity;
  int size;
  int (*hash)(const char *);
  entry *table;
};

#define SMAP_INITIAL_CAPACITY 25
#define RESIZE_FACTOR 2

#define ADD_NEW 2
#define ADD_UPDATE 4
#define ADD_FAILURE 6

int smap_compute_index(const char* key, int (* hash)(const char*), int cap);
int smap_table_add(entry* table, const char* key, int* value, int (*hash)(const char*), int capacity);
void smap_embiggen(smap* m);
int smap_table_find_key(const entry* table, const char* key, int (*hash)(const char*), int capacity);

// this bit creates the meta data
// the metadata has the capacity of the array, the initial size 0 and a entry_data pointer
smap *smap_create(int (*h)(const char *s))
{
  smap *result = malloc(sizeof(smap));
  if (result != NULL)
    {
      result->size = 0;
      result->hash = h;
      result->table = malloc(sizeof(entry) * SMAP_INITIAL_CAPACITY);
      result->capacity = (result->table != NULL ? SMAP_INITIAL_CAPACITY : 0);
      for(int i = 0; i < result->capacity; i++)
      {
          result->table[i].occupied = false;
      }
    }
  return result;
}

int smap_size(const smap *m)
{
  return m->size;
}

/**
 * Returns the index where key is located in the given map, or the index
 * where it would go if it is not present.
 *
 * @param table a table with at least one free slot
 * @param key a string, non-NULL
 * @param hash a hash function for strings
 * @param size the size of the table
 * @param capacity the capacity of the table
 * @return the index of key in table, or the index of the empty slot to put it in if it is not present
 */
int smap_table_find_key(const entry *table, const char* key, int (*hash)(const char*), int capacity)
{
    // compute starting location for search from hash function
    int i = smap_compute_index(key, hash, capacity);

    // if table is occupied AND the key is not a match, move i forward to next available spot
    while(table[i].occupied && (strcmp(table[i].key, key) != 0))
    {
        i = (i + 1) % capacity;
    }
    return i;
}

// be more careful about where you return bools
bool smap_put(smap *m, const char* key, int *value)
{
    // if adding the next value, will make you go over half the capacity, embiggen everything
    // this isn't super great if you just wanted to update a value, but oh well
    if(m->size + 1 >= m->capacity / 2)
    {
        smap_embiggen(m);
    }

    int add_result = smap_table_add(m->table, key, value, m->hash, m->capacity);

    // if the add was successful, increment the size of the map
    if(add_result == ADD_NEW)
    {
        m->size++;
    }

    // return true or false depending on if the add failed
    if(add_result == ADD_FAILURE)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int smap_table_add(entry* table, const char* key, int* value, int (*hash)(const char*), int capacity)
{
    // remember that find_key gives the index of the matched key, or the next available spot
    int index = smap_table_find_key(table, key, hash, capacity);

    // so if you get the index of the matched key, just update the value
    if(table[index].occupied)
    {
        table[index].value = value;
        return ADD_UPDATE;
    }
    else // if you are at an empty spot, add the new key value pair
    {
        char* key_copy = strdup(key);

        if(key_copy != NULL)
        {
            table[index].key = key_copy;
            table[index].value = value;
            table[index].occupied = true;
            return ADD_NEW;
        }

        // if falling outside
        return ADD_FAILURE;
    }
}

void smap_embiggen(smap *m)
{
    // multiple the existing capacity
    int new_capacity = m->capacity * RESIZE_FACTOR;

    // create a new table with new capacity
    entry* new_table = malloc(sizeof(entry) * new_capacity);

    if(new_table == NULL) { printf("[SMAP_EMBIGGEN] ERROR\n"); return; }
    for(int i = 0; i < new_capacity; i++) { new_table[i].occupied = false; }

    // iterate through the old table's keys
    for (int i = 0; i < m->capacity; i++)
    {
        // check if the spot has a key-value pair
        if(m->table[i].occupied)
        {
            // add it to the new table
            int add_result = smap_table_add(new_table, m->table[i].key, m->table[i].value, m->hash, new_capacity);

            if(add_result == ADD_FAILURE)
            {
                printf("[SMAP_EMBIGGEN] ERROR\n");
            }

            // free the old key of which a copy is made (not the value)
            free(m->table[i].key);
        }
    }

    // update the capacity
    m->capacity = new_capacity;

    // free the old table
    entry* old_table = m->table;
    free(old_table);

    // link the new table to smap
    m->table = new_table;
}

int smap_compute_index(const char* key, int (*hash)(const char*), int cap)
{
  return abs(hash(key) % cap);
}

bool smap_contains_key(const smap *m, const char* key)
{
    int index = smap_table_find_key(m->table, key, m->hash, m->capacity);
    return m->table[index].occupied;
}

int *smap_get(smap *m, const char* key)
{
    int index = smap_table_find_key(m->table, key, m->hash, m->capacity);
    if (m->table[index].occupied)
    {
        return m->table[index].value;
    }
    else
    {
        return NULL;
    }
}

void smap_for_each(smap *m, void (*f)(const char *, int *))
{
    for(int i = 0; i < m->capacity; i++)
    {
        if (m->table[i].occupied)
        {
            f(m->table[i].key, m->table[i].value);
        }
    }
}

void smap_for_each_r(smap *m, void (*f)(const char *, int *, void *), void *arg)
{
    for (int i = 0; i < m->capacity; i++)
    {
        if(m->table[i].occupied)
        {
            f(m->table[i].key, m->table[i].value, arg);
        }
    }
}

void smap_destroy(smap *m)
{
    // free all the keys (not our responsibility to free values)
    for (int i = 0; i < m->capacity; i++)
    {
        if (m->table[i].occupied)
        {
            free(m->table[i].key);
        }
    }

    // free the table and metadata
    free(m->table);
    free(m);
}
