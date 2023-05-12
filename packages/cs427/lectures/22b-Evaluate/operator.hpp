// =========================================================================
// A. Fischer, June 9, 2002                               file: operator.hpp
//
#pragma once

#include "tools.hpp"
class Operator {
  private: // --------------------------------------------------------------
    char symb;
    int  prec;

  public: // ---------------------------------------------------------------
    Operator( char op = '!') : symb(op) {
        switch (op){
          case '+': case '-':           prec = 1; break;
          case '*': case '/': case '%': prec = 2; break;
          case '^':                     prec = 3; break;
          default:                      prec = -1; break;
        }
    }
    ~Operator() {}
    int precedence() const { return prec; }
    char symbol()    const { return symb; }
    ostream& print(ostream& out)
        { return out <<"Symbol: " <<symb <<"  Precedence: " <<prec <<endl; }
};
inline ostream& operator<<( ostream & out, Operator& op) {return op.print(out); }

