// list based on a variable length array
// specialized to carry file metadata 

#define _GNU_SOURCE
// #include <linux/limits.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#define LIST_SIZE 10
#define RESIZE_FACTOR 2

// Write message to stderr using format FORMAT
#define LIST_WARN(format, ...) fprintf(stderr, "farthing: " format "\n", __VA_ARGS__)

// Write message to stderr using format FORMAT and exit.
#define LIST_DIE(format, ...) WARN(format, __VA_ARGS__), exit(EXIT_FAILURE)

typedef struct list_item
{
    char local_name[PATH_MAX]; // allocated on the stack/does not need to be freed
    int byte_size;
    int local_name_len;
    bool is_dir;
    bool is_lnk;
    bool mark; // mark items of interest. useful for tKey
    
    // crucially, we do not store file contents, only metadata
} list_item;

typedef struct list
{
    int count;                   // no of items added to list
    int cap;                     // the capacity of the list
    list_item* items;            // list of list_items (heap allocated)

} list;

/**
 * @brief : initializes a stack-allocated list in place
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
 * @brief : pushes a list_item to the end of list l, AVOIDING DUPLICATES
 * 
 * @param l : the list being modified in place
 * @param item : the items being copied into the list
 */
void lPush(list* l, list_item item);

/**
 * @brief : checks name of list item against list for name match
 * A name match is: all chars must match (excluding any trailing slashes)
 * 
 * @param l : the list being checked
 * @param item : item to check against list
 * @return : true if match, else return false
 */
bool lFind(list* l, list_item item);

/**
 * @brief : retrieves an item at a particular index from a list
 * 
 * @param l : the list being inspected
 * @param index : the index from which to retrieve the item:
 * @return : ptr to list item if valid index, else return NULL
 */
list_item* lGet(list* l, int index);

/**
 * @brief : copies l1 into l2
 */
void liCopy(list_item* l1, list_item* l2);

/**
 * @brief : doubles the size of the list when half-full
 * 
 * @param l : the list to expand
 */
void lExpand(list* l);

/**
 * @brief : print contents of list in friendly form
 * 
 * @param l : the list to print
 */
void lPrint(list *l, char* mode);

/**
 * @brief : return the number of list_items in a list
 * 
 * @param l : the list 
 * @return int : the number of items
 */
int lCount(list *l);

/**
 * @brief : matches l1 against l2 by name
 * if any item in l1 has the same name as the item in l2, then the item in l2 is marked
 * if any item in l1 has a name that matches the item in l2, then the item in l2 is marked
 * 
 * @param l1 : list 
 * @param l2 : list
 */
void lMatch(list *l1, list *l2);

/**
 * @brief : since the order that the items appear in the archive does
 * not matter, this function works by swapping the item to delete with
 * the last item in the list, and freeing the last item. This avoids
 * having to shift down everyone else.
 * 
 * @param l : the list to delete from
 * @param index : the index of the list_item to delete
 */
void lDelete(list*l, int index);

/**
 * @brief : initializes a stack-allocated list_item in place
 * 
 * @param li : the list item
 */
void liSetup(list_item* li);

/**
 * @brief : reads a local name into a list item
 * It assumes all error checking has been done by user 
 * (i.e the local name is valid)
 * 
 * It also assumes the list item has been setup
 * 
 * @param local_name : the name of the file to do lstat on
 * @param li : the list item to modify in place
 */
void liRead(list_item *li, char *local_name);

/* TRAILING SLASHES SHOULDN'T HAVE TO BE A PAIN */

/**
 * @brief : eliminate trailing slashes
 *
 * @param name : str to eliminate slashes from
 * @return true : if atleast one slash was removed
 * @return false : otherwise
 */
bool eliminateTrailingSlashes(char* name);

/**
 * @brief : eliminate trailing slashes list
 *
 * @param namse : list of strs to eliminate slashes from
 * @return true : if atleast one slash was removed
 * @return false : otherwise
 */
void eliminateTrailingSlashesList(char** names, int names_count);

/**
 * @brief : checks if last char of a str is a slash
 *
 * @param str
 * @return true : if it is
 * @return false : if not
 */
bool isTrailingSlash(char* str);
