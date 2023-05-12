// ============================================================================
// Name: Bracket-matching example of stack usage             File: brackets.hpp
// Author:  Michael and Alice Fischer                   Copyright: January 2009
// Modified September 2018
// ============================================================================
#ifndef BRACKETS_H
#define BRACKETS_H

#include "tools.hpp"
#include "token.hpp"
#include "stack.hpp"

class Brackets {
private:
    Stack& stk = *new Stack("brackets");
    int lineno = 1;

public:
    Brackets() =default;
    ~Brackets() { delete &stk; }
    Brackets( const Brackets& )            =delete;     // No copy constructor
    Brackets& operator=( const Brackets& ) =delete;     // No copy assignment
    Brackets( Brackets&& )                 =default;    // Default move constructor
    Brackets& operator=( Brackets&& )      =default;    // Default move assignment

    void checkFile( istream& in ); // Check bracket nesting and matching in file.
    bool checkChar( char ch );     // Check next character
};
#endif
