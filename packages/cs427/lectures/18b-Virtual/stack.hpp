// -------------------------------------------------------------------------
// Stacks, with an inheritance hierarchy
// A. Fischer  June 8, 2001                                  file: stack.hpp
// -------------------------------------------------------------------------
#pragma once

#include "linear.hpp"

// -------------------------------------------------------------------------
class Stack : public Linear {
  public:
    Stack(){}
    ~Stack(){}
    void  insert( Cell* cp ) { reset(); Linear::insert(cp); }
    void  focus(){ reset(); }

    ostream& print( ostream& out ){
        out << "  The stack contains:\n";
        return Linear::print( out );
    }
};
