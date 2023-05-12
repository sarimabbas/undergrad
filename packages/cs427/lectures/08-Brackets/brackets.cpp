// ============================================================================
// Name: Bracket-matching example of stack usage             File: brackets.cpp
// Author:  Michael and Alice Fischer                   Copyright: January 2009
// Modified September 2016
// ============================================================================
#include "brackets.hpp"
//-----------------------------------------------------------------------------
void Brackets::
checkFile(istream& in) {
    bool ok = true;
    for (;;) {                  // Read and process the file.
        char ch;
        in.get(ch);             // This does not skip leading whitespace.
        if (!in.good())
            break;
        if (ch == '\n')
            lineno++;
        ok = checkChar( ch ) && ok;
    }
    if (!in.eof())
        fatal("Failure reading input character");
    if (!stk.empty()) {
        cout << "\n" << "Unmatched left brackets at EOF" << endl;
        cout << stk << endl;                           // print stack contents
        ok = false;
    }
    if (ok)
        cout << "\nBrackets in this file are properly nested and matched.\n";
    else
        cout << "\nOne or more bracket mismatches in this file.\n" << endl;
}

//-----------------------------------------------------------------------------
bool Brackets::
checkChar( char ch ) {
    Token curtok(ch);

    // Nothing to do for non-brackets
    if (!curtok.isBracket()) return true;

    // Left brackets get pushed onto the stack
    if (curtok.isLeft()) {
        stk.push(curtok);
        return true;
    }

    // Right bracket is excess if stack is empty
    if (stk.empty()) {
        cout << "\n" << "Extra right bracket '" << curtok
                << "' on line " << lineno << " discarded." <<endl;
        return false;
    }

    // Right bracket should match top of stack
    Token toptok = stk.peek();
    if ( !toptok.sameTypeAs(curtok)) {
        cout << "\n" << "Left bracket '" << toptok
                << "' does not match right bracket '" << curtok
                << "' on line " << lineno << endl;
        cout << stk << endl;                            // print stack contents
        cout << "Both brackets discarded." << endl;
        stk.pop();  // pop matching left bracket from stack
        return false;
    }

    stk.pop();  // pop matching left bracket from stack
    return true;
}

