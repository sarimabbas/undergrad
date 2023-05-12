/*  ---------------------------------------------------------------------------
//  Main program for Sorting DataPacks.                                2_main.c
//  Created by Alice Fischer on Mon Dec 22 2003.
//  -------------------------------------------------------------------------*/
#include "datapack.h"

/*  -------------------------------------------------------------------------*/
int main( void )
{
    DataPack theData;
    banner();
    say( "Construct pack, read data" );     setup( &theData );
    say( "\n%d data items read:",           theData.n );
    say( "\nUnsorted data:" );              printData( &theData, stdout );
    say( "\nBeginning to sort." );          sortData( &theData );
    say( "\nSorted results:" );             printData( &theData, stdout );
    bye();
    return 0;
}
