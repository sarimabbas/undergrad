//===========================================================================
//  Exam: A student's initials and one exam score.
//  A. Fischer, October 1, 2000                                file: exam.hpp
//===========================================================================
#pragma once

#include <iostream>
#include <cstring>
using namespace std;

typedef int KeyType;    // Instantiate KeyType of Item

//---------------------------------------------------------------------------
class Exam                      // One name-score pair
{
  private:  //---------------------------------------------------------------
    char initials [4];          // Array of char for student name
  protected://---------------------------------------------------------------
    int score;                  // Integer to hold score
  public:  //----------------------------------------------------------------
    Exam (const char* init, int sc){
        strncpy( initials, init, 3 );
        initials[3] = '\0';
        score = sc;
    }
    ~Exam (){ cout << "    Deleting Score " <<initials <<"..."; }
    ostream& Print ( ostream& os ){
        return os <<initials <<": " <<score <<"  ";
    }
};
//---------------------------------------------------------------------------
inline ostream& operator << (ostream& out, Exam& T){ return T.Print( out ); }
