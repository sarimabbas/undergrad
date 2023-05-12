// ===================================================================================
// A. Fischer, June 9, 2002                                             file: eval.cpp
//
#include "eval.hpp"
#include "operator.hpp"

// ---------------------------------------------------- Instructions for the operator.
void Eval::instructions( void ){
    cout << "This is an infix expression evaluator.\n"
         << "* Operands start with a digit and may or may not have a decimal point.\n"
         << "* Operands must not exceed 31 keystrokes.\n"
         << "* All operators are binary operators.  Parentheses are not supported.\n"
         << "* Operators are: +, -, *, /, % (a mod b) and ^ (a to the power b).\n"
         << "* End each expression with a semicolon.\n\n"
         << "To quit, type semicolon instead of an expression.\n";
}
//------------------------------------------------ Read input and evaluate expression.
double
Eval::evaluate( istream& in ) {
    Intype next;    // Classification of next input character.
    Operator inOp;  // Operator object constructed from inSymbol.
    double inNum;   // Read input operands into this.
    char ch;

    for(;;) {
        in >> ws >>ch;
        if (in.eof()) next = end;
        else next = classify( ch );
        switch( next ){
          case number:
                in.putback(ch);
                in >> inNum;
                if ( Ands.depth() != Ators.depth() ) return expError();
                Ands.push( inNum );
                break;

          case op:
                inOp = Operator(ch);
                if ( Ands.depth() != Ators.depth()+1 ) return expError();
                force( inOp.precedence() );
                Ators.push( inOp );
                break;

          case end:
                if ( Ands.depth() != Ators.depth()+1 ) return expError();
                force( 0 );
                return  Ands.pop();
                break;

          case bad:
          default: return expError();
        }
    }
}

// -------------------------------- Evaluate all higher precedence operators on stack.
void
Eval::force( int rprec ) {
    while( Ators.depth()>0 && Ators.top().precedence() >= rprec ) dispatch();
}

//---------- Decide whether next input char is an operator, a semicolon, the beginning
Eval::Intype                                             // of an operand, or garbage.
Eval::classify( char ch ){
    if (isdigit( ch )) return number;
    switch(ch){
      case '+':
      case '-':
      case '*':
      case '/':
      case '%':
      case '^': return op;
      case ';': return end;
      default : return bad;
    }
}

// ------------------------------------------------------------ Evaluate one operator.
void
Eval::dispatch() {
    double result;
    double right = Ands.pop();
    double left = Ands.pop();
    Operator op = Ators.pop();
    switch (op.symbol()) {
        case '+': result = left + right;        break;
        case '-': result = left - right;        break;
        case '*': result = left * right;        break;
        case '/': result = left / right;        break;
        case '%': result = fmod(left, right);   break;
        case '^': result = pow (left, right);   break;
        default : result = NAN;                 break;
    }
    Ands.push( result );
}

// ------------------------------------------------------------------- Error comments.
double
Eval::expError(){
    cout << "\tIllegal expression.\n";
    print(cout);
    return HUGE_VAL;
}

// ----------------------------------------------------------------- Print the stacks.
ostream&
Eval::print( ostream& out ){
    out << "\tRemaining contents of operator stack: ";
    out << "\tRemaining contents of operand stack: " <<Ands;
    return out;
}
