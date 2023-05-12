#include "list.h"

// * does not seem to be leaking 
void lSetup(list* l) {
    // set the number of items to 0
    l->count = 0;
    // set the initial list size
    l->cap = LIST_SIZE;
    // set aside space for the list_items
    l->items = malloc(sizeof(list_item) * l->cap);
    // for each item in the list
    for(int i = 0; i < l->cap; i++) {
        // set the item's byte_size and local_name_len= 0
        liSetup(&(l->items[i]));
    }
}

void lDestroy(list *l) {
    // we only need to free the pointers
    // the list_items themselves are stack allocated
    free(l->items);
}

void lPush(list *l, list_item item) {
    // prevent adding duplicate
    if(lFind(l, item)) { return; }
    // check if pushing an item would require expansion
    if (l->count == l->cap) { lExpand(l); }
    // after expansion, we still want to continue adding the item
    // we do this by copying the item, reducing headache for everyone involved
    if (l->count < l->cap) {
        list_item* empty = &(l->items[l->count]);
        liSetup(empty);
        liCopy(&item, empty);
        l->count++;
    }
}

void lExpand(list *l) {
    int new_cap = l->cap * RESIZE_FACTOR;
    list_item* new_items = realloc(l->items, new_cap * sizeof(list_item));
    if (new_items != NULL) {
        l->items = new_items;
        l->cap = new_cap;
    }
}

list_item* lGet(list *l, int index) {
    if(index < l->count) {
        return &(l->items[index]);
    }
    else {
        return NULL;
    }
} 

void liCopy(list_item* l1, list_item* l2) {
    l2->local_name_len = l1->local_name_len;
    l2->byte_size = l1->byte_size;
    l2->is_dir = l1->is_dir;
    l2->is_lnk = l1->is_lnk;
    l2->mark = l1->mark;
    strcat(l2->local_name, l1->local_name);
}

bool lFind(list *l, list_item item) {
    eliminateTrailingSlashes(item.local_name);
    for (int i = 0; i < l->count; i++) {
        list_item curr; 
        liSetup(&curr);
        liCopy(lGet(l, i), &curr);
        eliminateTrailingSlashes(curr.local_name);
        if (strcmp(item.local_name, curr.local_name) == 0) {
            return true;
        }
    }
    return false;
}

int lCount(list *l) {
    return l->count;
}

void lDelete(list *l, int index) {
    // get the item to delete and the last item
    list_item* last = lGet(l, l->count - 1);
    list_item* at_index = lGet(l, index);
    // begin copying the last item to the current index
    at_index->byte_size = last->byte_size;
    at_index->local_name_len = last->local_name_len;
    // nullify the name of the current index
    for(int i = 0; i < strlen(at_index->local_name); i++) {
        at_index->local_name[i] = '\0';
    }
    // copy over the string from the last item
    strcpy(at_index->local_name, last->local_name);
    // now free the last block
    free(last);
    // decrement the count
    l->count--;
}

void liSetup(list_item *li) {
    for(int i = 0; i < PATH_MAX; i++) {
        li->local_name[i] = '\0';
    }
    li->local_name_len = 0;
    li->byte_size = 0;
    li->is_dir = false;
    li->is_lnk = false;
    li->mark = false;
}

void liRead(list_item* li, char* local_name) {
    // sanitize
    char name_cpy[PATH_MAX] = {'\0'};
    strcat(name_cpy, local_name);
    eliminateTrailingSlashes(name_cpy);
    // setup
    struct stat statbuf;
    if (lstat(name_cpy, &statbuf) != 0) {
        LIST_WARN("liRead for %s failed", name_cpy);
        return;
    }
    // store local name and len
    strcat(li->local_name, name_cpy);
    li->local_name_len = strlen(li->local_name);
    // store if directory or link, and byte size
    if (S_ISDIR(statbuf.st_mode))
    {
        li->byte_size = 0;
        li->is_dir = true;
        strcat(li->local_name, "/");
        li->local_name_len++;
    }
    else if (S_ISLNK(statbuf.st_mode))
    {
        li->byte_size = 0;
        li->is_lnk = true;
    }
    else
    {
        li->byte_size = statbuf.st_size;
    }
}

void lMatch(list *l1, list *l2) {
    for(int i = 0; i < lCount(l1); i++) {
        list_item* li1 = lGet(l1, i);
        for(int j = 0; j < lCount(l2); j++) {
            list_item* li2 = lGet(l2, j);
            if (strncmp(li1->local_name, li2->local_name, strlen(li1->local_name)) == 0) {
                li1->mark = true;
                li2->mark = true;
            }
        }
    }
}

void lPrint(list* l, char* mode) {
    for(int i = 0; i < lCount(l); i++) {
        list_item* li = lGet(l, i);
        if (strcmp(mode, "mark") == 0) {
            if (li->mark) {
                printf("%8d %s\n", li->byte_size, li->local_name);
            }
        }
        else {
            printf("%8d %s\n", li->byte_size, li->local_name);
        }
    }
}

/* TRAILING SLASHES SHOULDN'T HAVE TO BE A PAIN */

void eliminateTrailingSlashesList(char** names, int names_count) {
    if (names == NULL || names_count == 0) {
        return;
    } else {
        for (int i = 0; i < names_count; i++) {
            eliminateTrailingSlashes(names[i]);
        }
    }
}

bool eliminateTrailingSlashes(char* name) {
    bool slash_eliminated = false;
    if (name != NULL) {
        for (int i = strlen(name) - 1; i > -1; i--) {
            if (name[i] == '/') {
                name[i] = '\0';
            } else {
                slash_eliminated = true;
                return slash_eliminated;
            }
        }
        slash_eliminated = true;
    }
    return slash_eliminated;
}

bool isTrailingSlash(char* str) {
    if ('/' == str[strlen(str) - 1]) {
        return true;
    } else {
        return false;
    }
}
