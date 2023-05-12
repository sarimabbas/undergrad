// -----------------------------------------------------------------------------
// FlexArray class implementation                                       flex.cpp
// A. Fischer and M. Fischer, September 2010
//      Revised September 2018
// -----------------------------------------------------------------------------
// Function definitions for arrays that can grow.  
// The base type of the array is controlled by a typedef.

#include "flex.hpp"
//------------------------------------------------------------------------------
// Pre:  There is a newly allocated but uninitialized FlexArray.
// Post: The flexarray is internally consistent, empty, and ready to use. 
//
Flex::Flex( int initial )  : max(initial), ar(new ET[max]) {
    cout <<">>> New FlexArray, size = " <<max <<"\n";
}

Flex::~Flex() {
    delete [] ar;
    cout <<">>> Deleted FlexArray, size = " <<max <<"\n";
}

//------------------------------------------------------------------------------
// Called when FlexArray is full.  Doubles allocation size to create empty slots
// Being private prevents this function from being called outside the class.
// Pre:  max>0.
// Post: The length of FlexArray has been doubled.
// Error: System thows an exception in case of an allocation error.
//
void Flex::grow() {
    ET* temp = ar;
    ar = new Item[ max*=2 ];
    for (int k=0; k<n; ++k) ar[k] = move( temp[k] );
    delete [] temp;
    cout <<">>> Grew FlexArray, size now = " <<max <<"\n";
}

//------------------------------------------------------------------------------
// Pre:   d is a data object ready to put into the array.
// Post:  d is the last object in the array and n has been incremented.
//        An allocation failure throws an exception.
//
void Flex::add( ET&& d ) {
    if (n == max) grow();       // Enlarge array if it is full.
    ar[n++] = move( d );        // Normal case: put data into the array.
}

//------------------------------------------------------------------------------
// Pre:  Flexarray has been initialized and might contain data.
// Post: A heading and all the data in flexarray has been displayed on the screen.
//
ostream& Flex::print( ostream& out ) const {
    int k;
    out <<"Array size " <<max <<"; length " << n << "; contents:\n";
    for (k=0; k<n; ++k)  out <<"    " <<k <<": " <<ar[k] <<"\n";
    out <<"----------------------------\n";
    return out;
}

//------------------------------------------------------------------------------
// Delegate the sorting task to the std::sort.
// Pre:  Flexarray has been initialized and might contain data.
// Post: The data in FlexArray is sorted according to the rule for type BT.
//
void Flex::sort() {
    std::sort(ar, ar+n);
}

//------------------------------------------------------------------------------
// Pre:  FlexArray has been initialized and might contain data.
// Post: The data in the FlexArray is not changed.
// Return value:  If found, the position of the key; otherwise, -1.
//
int Flex::search( const string& key ) const {
    for (int k=0; k<n; ++k) {
        if (ar[k].match(key)) return k;
    }
    return -1;
}

// Local Variables:
// tab-width: 4
// End:
