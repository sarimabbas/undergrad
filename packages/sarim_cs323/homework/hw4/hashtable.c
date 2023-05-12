#include "hashtable.h"
#include "code.h"
#include "helper.h"

int PREF(int code, hashtable** htp) {
    if (code == RESIZE_SIGNAL) {
        return -1;
    }

    if (code == RESET_SIGNAL) {
        return -1;
    }

    if (code == EMPTY) {
        return EMPTY;
    }

    if (code == UNOCCUPIED) {
        return -1;
    }
    
    if (code < 0 || code > (*htp)->capdec) {
        return -1;
    }

    return (*htp)->table[code].prefix;
}

int CHAR(int code, hashtable** htp) {
    if (code == RESIZE_SIGNAL) {
        return 0;
    }

    if (code == RESET_SIGNAL) {
        return 0;
    }

    if (code == EMPTY) {
        return 0;
    }

    if (code == UNOCCUPIED) {
        return 0;
    }

    return (*htp)->table[code].character;
}

bool KNOWN(int code, hashtable** htp) {
    return (*htp)->table[code].prefix != UNOCCUPIED;
}

/**
 * @brief 
 * 
 * @param capbits : the starting size
 * @param maxbits : maximum allowable size
 * @return hashtable* 
 */
hashtable* htCreate(int capbits, int maxbits) {

    hashtable* result = malloc(sizeof(hashtable));

    if (result == NULL) {
        return NULL;
    }

    // convert power of 2 to decimal capacity
    int capdec = bit2Dec(capbits);

    // update
    result->maxbits = maxbits;
    result->capbits = capbits;
    result->capdec = capdec;
    result->size = 0;

    // allocate table
    result->table = malloc(sizeof(entry) * capdec);

    if (result->table == NULL) {
        free(result);
        return NULL;
    }

    // init entries
    for (int i = 0; i < result->capdec; i++) {
        eUnoccupy(&(result->table[i]));
    }

    return result;
}

void htInitTable(hashtable** htp) {
    htInitSignals(htp); // adds 2 to the size
    htInitAscii(htp);
}

void htInitSignals(hashtable** htp) {
    hashtable* ht = *htp;
    ht->table[RESIZE_SIGNAL].prefix = RESIZE_SIGNAL;
    ht->table[RESIZE_SIGNAL].character = 0;

    ht->table[RESET_SIGNAL].prefix = RESET_SIGNAL;
    ht->table[RESET_SIGNAL].character = 0;

    ht->table[EMPTY].prefix = 1;
    ht->table[EMPTY].character = 0;
    
    ht->size += 3;
}

void htInitAscii(hashtable** htp) {
    // all the other characters (duplicate checking built-in)
    for (int i = 0; i < 256; i++) {
        entry init;
        init.character = i;
        init.prefix = EMPTY;
        htAdd(htp, &init);
    }
}

int htFind(hashtable** htp, entry* e) {
    hashtable* ht = *htp;

    int pos = htHypothesize(htp, e);

    if (pos == -1) {
        return -1;
    }

    if (ht->table[pos].prefix == UNOCCUPIED) {
        return -1;
    }

    return pos;
}

int htHypothesize(hashtable** htp, entry *e) {
    hashtable* ht = *htp;
    int start = RED(INIT(e->prefix, e->character), ht->capdec);
    int pos = start;
    int curr_character;
    int curr_prefix;

    while (ht->table[pos].prefix != UNOCCUPIED) {

        curr_character = ht->table[pos].character;
        curr_prefix = ht->table[pos].prefix;

        // if equal to entry
        if (e->prefix == curr_prefix && e->character == curr_character) {
            return pos;
        }

        // step
        pos = RED(pos + STEP(e->prefix, e->character), ht->capdec);

        // prevent cycles
        if (pos == start) {
            return -1;
        }
    }

    return pos;
}

int htAdd(hashtable** htp, entry* e) {
    // prepare
    hashtable* ht = *htp;

    int pos = htHypothesize(htp, e);

    // if above failed
    if (pos == -1) {
        return -1;
    }

    // if already present
    if (ht->table[pos].prefix == e->prefix && ht->table[pos].character == e->character) {
        return pos;
    }

    // if not present, but space is occupied
    if (ht->table[pos].prefix != UNOCCUPIED) {
        return -1;
    }

    // slot is guaranteed to be empty, and entry is not in the table
    eTransfer(e, &(ht->table[pos]));

    // increment count
    ht->size++;

    // return what was just added
    return pos;
}

bool htGrowEminent(hashtable** htp) {
    hashtable* ht = *htp;

    float load_average = ht->size / (float) ht->capdec;

    // if the load average is below threshold already
    if (load_average < GROW_THRESHOLD) {
        return false;
    }

    // if doubling the size would exceed specified MAXBITS
    if (ht->capbits + 1 > ht->maxbits) {
        return false;
    }

    // otherwise, trigger a resize
    return true;
}

bool htCanAdd(hashtable** htp) {
    hashtable* ht = *htp;
    float load_average = ht->size / (float)ht->capdec;
    if (load_average > GROW_THRESHOLD + 0.0005) {
        return false;
    } else {
        return true;
    }
}

int* htGrow(hashtable** htp) {
    // create old pointer
    hashtable* old = *htp;

    // create new hashtable with double the capacity but same maxbits 
    hashtable* new = htCreate(old->capbits + 1, old->maxbits);
    // hashtable** newp = &new;

    // init the signals
    htInitSignals(&new);

    // create a prefix conversion table of old table's capacity, initialized to UNOCCUPIED
    int* conversion_table = malloc(sizeof(int) * old->capdec);
    for(int i = 0; i < old->capdec; i++) { 
        conversion_table[i] = UNOCCUPIED; 
    }

    // init the first three codes (base case)
    conversion_table[0] = RESIZE_SIGNAL;
    conversion_table[1] = RESET_SIGNAL;
    conversion_table[2] = EMPTY;

    // convertTableDump(conversion_table, old->size);

    // iterate through the old hashtable's entries
    for(int i = 3; i < old->capdec; i++) {
        
        // if the spot is occupied in the old
        if (old->table[i].prefix != UNOCCUPIED) {
            
            // pass to the recursive helper to add to the new table
            htGrowRecurse(i, htp, &new, conversion_table);
        }
    }

    // free the old hashtable and repoint
    htDestroy(htp);
    *htp = new;

    // return conversion table
    return conversion_table;
}

void htReset(hashtable** htp) {
    // take what you need from the old guy
    hashtable* old = *htp;
    int capbits = old->capbits;
    int maxbits = old->maxbits;
    htDestroy(htp);

    // create new table
    hashtable* new = htCreate(capbits, maxbits);

    // init signals and ASCII
    htInitTable(&new);

    // repoint
    *htp = new;
}


// @brief : adds an entry (given by entry_pos) into hashtable newp
void htGrowRecurse(int entry_pos, hashtable** oldp, hashtable** newp, int* conversion_table) {
    // prepare
    hashtable* old = *oldp;

    // get entry
    entry e = old->table[entry_pos];

    // if table does not have equivalence for prefix
    if (conversion_table[e.prefix] == UNOCCUPIED) {

        // call function on old pair with the code/position = prefix
        htGrowRecurse(e.prefix, oldp, newp, conversion_table);
    }

    // so from the previous recursion, we are guaranteed access to the new prefix
    // if there is still nothing, then throw an error :(
    if (conversion_table[e.prefix] == UNOCCUPIED) {
        fprintf(stderr, "htGrowRecurse: still could not find new prefix\n");
        return;
    }

    e.prefix = conversion_table[e.prefix];

    // add the entry to the table
    int pos = htAdd(newp, &e);

    // we now expect the entry exists in the table
    if (pos == -1) {
        fprintf(stderr, "htGrowRecurse: htAdd failed\n");
    }

    // update the conversion table with the new data
    int old_code = entry_pos;
    int new_code = pos;
    conversion_table[old_code] = new_code;
}

int htSize(hashtable** htp) {
    hashtable* ht = *htp;
    return ht->size;
}

void htDestroy(hashtable** htp) {
    hashtable* ht = *htp;
    free(ht->table);
    free(ht);
    *htp = NULL;
    htp = NULL;
}

void convertTableDump(int* conversion_table, int n) {
    fprintf(stderr, "%10s%10s\n", "INDEX/OLD PREFIX", "NEW PREFIX");
    for (int i = 0; i < n; i++) {
        fprintf(stderr, "%10i%10i\n", i, conversion_table[i]);
    }

}

void htDump(hashtable** htp, char* mode) {
    // prepare
    hashtable* ht = *htp;

    // headings
    fprintf(stderr, "%10s%10s%10s%10s\n", "CODE", "PREFIX", "CHAR", "STRING");

    // entries
    for (int i = 0; i < ht->capdec; i++) {

        int prefix = ht->table[i].prefix;

        int character = ht->table[i].character;

        stack* Kstack = stackBuilder(htp, prefix, character);

        if (strcmp(mode, "occupied") == 0) {
            if (ht->table[i].prefix != UNOCCUPIED) {
                fprintf(stderr, "%10i%10i%10i", i, prefix, character);
                sDump(Kstack);
            }
        } else {
            fprintf(stderr, "%10i%10i%10i", i, prefix, character);
            sDump(Kstack);
        }

        sDestroy(Kstack);
    }

    // separator
    fprintf(stderr, "-----------------------\n");
}

stack* stackBuilder(hashtable** htp, int prefix, int character) {
    
    stack* Kstack = sCreate();

    sPush(Kstack, character);

    int C = prefix;
    while(PREF(C, htp) != EMPTY) { 
        sPush(Kstack, CHAR(C, htp));
        C = PREF(C, htp);
    }

    int finalK = CHAR(C, htp);
    sPush(Kstack, finalK);

    return Kstack;
}

/** ENTRY CODE **/

void eUnoccupy(entry* e) {
    e->prefix = UNOCCUPIED;
    e->character = '\0';
}

bool eEqual(entry* e1, entry* e2) {
    if (e1->prefix == e2->prefix && e1->character == e2->character) {
        return true;
    } else {
        return false;
    }
}

void eTransfer(entry* src, entry* dest) {
    dest->character = src->character;
    dest->prefix = src->prefix;
}
