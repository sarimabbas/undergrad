#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code.h"
#include "stack.h"

#define INIT(p, c) (p)                                  // Initial slot
#define STEP(p, c) (2 * (((p) << CHAR_BIT) | (c)) + 1)  // Step to next slot
#define RED(x, m) ((x) & (m - 1))                       // Reduce to [0,M)

#define UNOCCUPIED -1
#define GROW_THRESHOLD 0.99
#define GROW_FACTOR 2

#define ABSOLUTE_MINBITS 9
#define ABSOLUTE_MAXBITS 22

#define EMPTY 2
#define RESET_SIGNAL 1
#define RESIZE_SIGNAL 0

typedef struct {
    int prefix: ABSOLUTE_MAXBITS; // bit fields
    int character:  ABSOLUTE_MINBITS;
} entry;

typedef struct {
    entry* table;
    int size;
    int capdec;

    int capbits;
    int maxbits;
} hashtable;

typedef struct {
    entry* entry;
    int pos;
} find;

hashtable* htCreate(int capbits, int maxbits);
void htInitTable(hashtable** htp);
void htInitSignals(hashtable** htp);
void htInitAscii(hashtable** htp);
int htSize(hashtable** htp);

int htHypothesize(hashtable** htp, entry* e);
int htFind(hashtable** htp, entry* e);
int htAdd(hashtable** htp, entry* e);
bool htCanAdd(hashtable** htp);

void htDestroy(hashtable** htp);
int* htGrow(hashtable** htp);
void htReset(hashtable** htp);
void htGrowRecurse(int entry_pos, hashtable** oldp, hashtable** newp, int* conversion_table);
bool htGrowEminent(hashtable** htp);

int PREF(int code, hashtable** htp);
int CHAR(int code, hashtable** htp);
bool KNOWN(int code, hashtable** htp);

void eUnoccupy(entry* e);
void eTransfer(entry* src, entry* dest);
bool eEqual(entry* e1, entry* e2);

/* DEBUG FUNCTIONS */
void convertTableDump(int* conversion_table, int n);
void htDump(hashtable** htp, char* mode);
stack* stackBuilder(hashtable** htp, int prefix, int character);
