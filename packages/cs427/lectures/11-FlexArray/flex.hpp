// -----------------------------------------------------------------------------
// FlexArray class header                                               flex.hpp
// A. Fischer and M. Fischer, September 2010
//      Revised September 2018
// -----------------------------------------------------------------------------
// Header file for arrays that can grow.
// The base type is controlled by a typedef for any predefined type or class.
//
#pragma once
#include "item.hpp"

//------------------------------------------------------------------------------
// ET is the element type of the FlexArray.
// ET must have the following operations defined for it:
// 1. ostream& operator<<( ostream&, const ET& );
// 2. bool operator<( const ET&, const ET& );
// 3. Move assignment:  ET& operator=( ET&& );
typedef Item ET ;

//------------------------------------------------------------------------------
class Flex {
private:
    int n=0;            // The number of objects currently stored in the array.
    int max;            // The number of array slots currently allocated.
    ET* ar;             // A dynamic array of size==max.
    void grow();

public:
    Flex( int initial=2 );
    ~Flex();

    void add( ET&& d );
    ET& operator[](int k) const { return ar[k]; }  // Subscript
    int size() const { return n; }
    ostream&  print( ostream& out ) const; // Output the whole array
    void sort();                           // Sort elements of the array
    int  search( const string& key ) const;  // Search for given element in array.
};

inline ostream& operator<<( ostream& out, Flex& f ){ return f.print( out ); }

// Local Variables:
// tab-width: 4
// End:
