// ========================================================================
// A. Fischer, June 9, 2002                                  file: eval.hpp
//
// Parse and evaluate a prefix expression.  All operators are binary;
// they are: +, -, *, /, % (a mod b) and ^ (a to the power b)
// Whitespace must be used to delimit both operators and operands.
// Operands must start with a digit and may or may not have a decimal point.
// Operands must not exceed 30 keystrokes.

#pragma once

#include "tools.hpp"
#include "stackT.hpp"
#include "operator.hpp"

class Eval {
  private: // --------------------------------------------------------------
    enum Intype { bad, number, op, end };
    Stack<double> Ands;     // Stack of operands and intermediate results.
    Stack<Operator> Ators;  // Stack of operators.
    Intype classify( char ch );
    void   dispatch();
    void   force( int rprec );
    double expError();

  public: // ---------------------------------------------------------------
    Eval(): Ands("Ands"), Ators("Ators") {};
    ~Eval(){}
    static void instructions( void );
    double evaluate( istream& in );
    ostream& print( ostream& out );
};
