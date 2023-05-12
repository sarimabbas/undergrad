//===========================================================================
//  Functions and constants for the Card class. 
//  Alice E. Fischer, March 2009                                cards.cpp
//===========================================================================
#include "cards.hpp"
const char* suitlabels[5] = {"spades", "hearts", "diamonds", "clubs", "bad"};

const char* spotlabels[16] = {
    "bad","Ace","2","3","4","5","6","7","8","9",
    "10","Jack","Queen","King","Ace" 
};

// --------------------------------------------------------------------------
void Card::instructions( ostream& out, int n) {
    out << "Please enter " << n << " cards.\n"
        << "Spot codes are 2..9, T, J, Q, K, A \n"
        << "Suit codes are S H D C \n";
}

// --------------------------------------------------------------------------
Card::Card (istream& sin) {
    char inspot, insuit;
    sin >> inspot >> insuit;
    if (!sin.good()) throw "Low level read error\n";
    if (inspot >='2' && inspot<='9') spot_ = inspot - '0';
    else switch( toupper(inspot) ){
        case 'T': spot_ = 10; break;
        case 'J': spot_ = 11; break;
        case 'Q': spot_ = 12; break;
        case 'K': spot_ = 13; break;
        case 'A': spot_ = 1;  break;
        default : spot_ = 0;  break;
    };
    switch( toupper(insuit) ){
        case 'S': suit_ = spades;   break;
        case 'H': suit_ = hearts;   break;
        case 'D': suit_ = diamonds; break;
        case 'C': suit_ = clubs;    break;
        default : suit_ = bad;      break;
    };
    if (spot_ == 0 && suit_ == bad) throw Bad(inspot, insuit);
    if (spot_ == 0)   throw BadSpot(inspot, insuit);        
    if (suit_ == bad) throw BadSuit(inspot, insuit);    
}

// --------------------------------------------------------------------------
ostream&
Card::print(ostream& sout) {
    return sout <<spotlabels[spot_] <<" of " <<suitlabels[suit_] <<endl;
}
