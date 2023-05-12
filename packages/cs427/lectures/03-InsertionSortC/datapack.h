/*  ----------------------------------------------------------------------
//  Header file for all DataPack programs.                          pack.h
//  Created by Alice Fischer on Mon Dec 22 2003.
*/
#include "tools.h"
/* ---------------------------------- Instantiations for generic parts  */
#define IN_FMT "%g"
#define OUT_FMT "%.7g\n"
typedef float BT ;
#define LENGTH 20

/* -------------------------------------------- Generic type definition  */
typedef struct { 
    int n;           /* The current number of items in the array. */
    int max;         /* Allocation length of the array. */
    BT* store;       /* For a dynamically allocated data array. */
} DataPack; 

/* -------------------------------------------------------- Prototypes  */
void setup( DataPack* pData );
void printData( DataPack* pData, FILE* outstream );
void sortData( DataPack* pData );
