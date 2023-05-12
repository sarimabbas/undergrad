// ============================================================================
// Name: Bracket-matching example of stack usage                File: token.cpp
// Author:  Michael and Alice Fischer                   Copyright: January 2009
// Modified September 2016
// ============================================================================
#include "token.hpp"

const string Token::brackets = "[](){}<>";

//-----------------------------------------------------------------------------
Token::
Token(char ch) :ch(ch) {
    classify(ch);
}

//-----------------------------------------------------------------------------
void Token::
classify(char ch) {
    const size_t pos = brackets.find_first_of( ch );
    if (pos == string::npos) {
        type = BKT_NONE;
        sense = SENSE_LEFT;              // arbitrary value
    } else {
        type = (BracketType) (pos / 2);  // integer arithmetic, with truncation.
        sense = (pos % 2 == 0) ? SENSE_LEFT : SENSE_RIGHT;
    }
}
