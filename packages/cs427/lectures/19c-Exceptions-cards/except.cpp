//==============================================================================
//  Exception handling demonstration.   
//  Alice E. Fischer, August 2009                                     except.cpp
//  Modified by Michael Fischer, November 2010
//==============================================================================

//#include "tools.hpp"
#include <iostream>
#include "cards.hpp"
#include "bad.hpp"
#define NCARDS 3

using namespace std;

//-----------------------------------------------------------------------------
int main( void )
{
    Card* hand[NCARDS];
    int k;
    Card::instructions( cout, NCARDS );
    //------------------------------------------ Main loop that reads all cards.
    for (k=0; k<NCARDS; ){              
        //--------------------------------------------- Here is the active code.
        try {                           
            cout << "\nEnter a card (spot code, suit code): " ;
            hand[k] = new Card(cin);    //---------------------- Input one card.
            cout << "  Card successfully entered into hand: ";
            hand[k]->print(cout);
            ++k;
        } 
        //----------------- Check for the three application-specific exceptions.
        catch (Bad& bs) { bs.print(); }          // Will catch all 3 Bad errors.

        //------------------- Now check for general exceptions thrown by system.
        catch (bad_alloc& bs) {         //----------------- Catch a new failure.
            cerr << "  Allocation error for card #" <<k <<".\n";
            return 1;                   //------------------------- Exit main().
        }
        catch (...) {                   //--------------- Catch everything else.
            cerr << "  Last-ditch effort to catch exceptions.\n";
            throw;                      //---------------- Can't go on, so punt.
        }
        // ----------------- Control comes here after the try/catch is finished.
    }   
    cout << "\nHand is complete:" << endl;
    for (k = 0; k < NCARDS; ++k) { hand[k]->print( cout ); }
    // Clean up
    for (k = 0; k < NCARDS; ++k) { delete hand[k]; }
}
