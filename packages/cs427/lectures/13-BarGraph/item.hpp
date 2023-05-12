//===========================================================================
// Item: A student's initials and one exam score.
// A. Fischer, October 1, 2000                                file: item.hpp
// Modified M. & A. Fischer, September 17, 2009
//===========================================================================
#pragma once
#include <iostream>
#include <string.h>
using namespace std;
//---------------------------------------------------------------------------
// Data class to model a student exam score
// Alice's design pattern:
//     oil and water don't mix -- keep data and data structure separate
// Constructor and destructor contain tracing printouts for debugging
class Item                      // One name-score pair
{
  private:                      // Variable names are private
    char initials [4];          // Array of char for student name
    int  score;                 // Integer to hold score

  public:
    inline Item (char* inits, int sc);
    ~Item (){ cerr <<"    Deleting Item " <<initials <<" ...\n"; }
    ostream& print ( ostream& os ) const;
};

//---------------------------------------------------------------------------
// Inline constructor, defined outside of the class but in header file
// Precondition: strlen(inits) <= 3
Item::Item (char* inits, int sc){
    strcpy( initials, inits );
    score = sc;
}

//---------------------------------------------------------------------------
inline ostream&    // inline can be declared here or within class (or both)
Item::print ( ostream& os ) const {
    return os <<initials <<" " <<score <<" ";
}

// Extend global output operator << -----------------------------------------
// Item::print() is expert at printing Item data
inline ostream&
operator << (ostream& out, const Item& x){ return x.print( out ); }
