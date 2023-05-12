// -----------------------------------------------------------------------------
//  Code file for all DataPack programs.                            pack.c
//  Created by Alice Fischer on Mon Dec 22 2003.
//  Modified by Michael Fischer, September 7, 2016

#include "datapack.h"
static void readData( DataPack* pData );   // A private function

//--------------------------------------------------------------------- 
// Allocate memory for data.  Errors are fatal.  

void 
setup( DataPack* pData ) 
{  
    pData->n = 0;                               // Currently empty. 
    pData->max = LENGTH;                        // Space for LENGTH items. 
    pData->store = malloc(LENGTH*sizeof(BT)); 
    if (pData->store == NULL) 
        fatal( " Error: not enough memory for %i BTs\n", LENGTH ); 
    readData( pData );  
}

//-----------------------------------------------------------------------------
// Use sequential access pattern to store data from infile into data pack.   
 
static void 
readData( DataPack* pData )
{   
    char filename[80];              // For name of input file, 79 char max
    FILE* infile;
    BT* cursor, * end;
    int status;
    
    printf( "\nEnter name of data file to be searched: " );
    scanf( "%79s", filename );
    infile = fopen ( filename, "r" ); 
    if (!infile)  fatal( "    Error: couldn't open input %s\n", filename );
    
    cursor = pData->store;          // Scanning pointer, set to start of array.     
    end = pData->store + pData->max;    // An off-board sentinel 
    
    for( ; cursor<end; ++ cursor ) {
        status = fscanf( infile, IN_FMT, cursor);
        if( status != 1 ) break;    // Quit for bad data or for end of file.
    }
    pData->n = cursor-pData->store; // Actual # of items read. 
    fclose( infile );
}

// ------------------------------------------------------------------------
// Print array values, one per line, to the selected stream.
 
void 
printData( DataPack* pData, FILE* outstream )
{     
    for( int k=0; k < pData->n; ++k)  
        fprintf( outstream, OUT_FMT, pData->store[k] );     
}

// ------------------------------------------------------------------------
// Generic insertion sort using a DataPack.            
// Sort n values starting at pData->store by an insertion sort algorithm. 

void
sortData( DataPack* pData )  
{
    BT* const end = pData->store + pData->n;    // Off-board sentinel. 
    BT* pass;                           // First unsorted item; begin pass here. 
    BT  newcomer;                       // Data value being inserted. 
    BT* hole;                           // Array slot containing no data. 

    for ( pass=pData->store+1; pass<end; ++pass ) { 
        // Pick up next item and insert into sorted portion of array. 
        newcomer = *pass;
        for ( hole=pass; hole>pData->store; --hole ) {
            if ( *(hole-1) <= newcomer ) break;   // Insertion slot is found. 
            *hole = *(hole-1);                    // Move item back one slot. 
        }
        *hole = newcomer;
    }
}
