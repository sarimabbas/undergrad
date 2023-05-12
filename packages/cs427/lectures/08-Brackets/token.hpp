// ============================================================================
// Project: Bracket-matching example of stack usage             File: token.hpp
// Author:  Michael and Alice Fischer                   Copyright: January 2009
// Modified September 2016
// ============================================================================
#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "tools.hpp"
#include <string>

class Token {
private:
    enum TokenSense {SENSE_LEFT, SENSE_RIGHT};
    enum BracketType {BKT_SQ, BKT_RND, BKT_CURLY, BKT_ANGLE, BKT_NONE};
    static const string brackets;

    BracketType type{BKT_NONE};
    TokenSense sense{SENSE_LEFT};
    char ch = '\0';
    void classify(char ch);

public:
    Token() =default;
    Token( char ch );
    ostream&    print( ostream& out)  const { return out << ch; }
    bool isLeft()                     const { return sense == SENSE_LEFT; }
    bool isBracket()                  const {return type!=BKT_NONE; }
    bool sameTypeAs(const Token& rhs) const { return type==rhs.type; }
};

inline ostream& operator<<( ostream& out, const Token& t ) {
    return t.print( out );
}
#endif // TOKEN_HPP
