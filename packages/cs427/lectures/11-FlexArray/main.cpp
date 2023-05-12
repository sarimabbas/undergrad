// -----------------------------------------------------------------------------
// FlexArray main program and unit test -- typedef version              main.cpp
// A. Fischer and M. Fischer, September 2010
//      Revised September 2018
// -----------------------------------------------------------------------------
// The base type of the array is controlled by a typedef.

#include "tools.hpp"
#include "flex.hpp"
#include "item.hpp"

void unitTest( void );

//------------------------------------------------------------------------------
int main( void ) {
    banner();
    puts( "FlexArray Demonstration Program" );
    unitTest();
    bye();
    return 0;
}

//------------------------------------------------------------------------------
void unitTest( void ) {
    cout << "Starting unit test for FlexArray" << endl;

    // Testing setup and print.
    cout << "Creating Flex object\n";
    Flex f;         // Primary data structure.
    cout <<"User should see array header and no data.\n";
    cout << f << endl;

    // Testing add and print.
    cout << "Adding 5 items\n";
    f.add( Item(6.00, "vollkornbrot") );
    f.add( Item(3.00, "croissant") );
    f.add( Item(2.50, "baguette") );
    f.add( Item(3.00, "danish") );
    f.add( Item(5.00, "anise bread" ) );
    cout <<"User should see " << f.size() << " items, not sorted.\n";
    cout <<f <<endl ;

    // Testing subscript
    cout << "Testing subscript\n";
    cout << "User should see baguette in slot 2 above.\n" << f[2] <<"\n" <<endl;

    // Testing sort
    cout << "Testing sort\n";
    f.sort( );
    cout << "User should see the data sorted by product name.\n";
    cout <<f <<endl ;

    // Testing search
    cout << "Testing search\n";
    cout << "Searching for \"croissant\".  Answer should be slot 2.\n";
    int found = f.search( "croissant" );
    cout <<"croissant found at slot " <<found <<".\n" <<endl;

    cout << "Searching for \"cupcake\".  Answer should be -1 (not found).\n";
    found = f.search( "cupcake" );
    cout <<"cupcake search returned slot " <<found <<".\n" <<endl;

    cout <<"Terminating unit test program" <<endl;
}

// Local Variables:
// tab-width: 4
// End:
