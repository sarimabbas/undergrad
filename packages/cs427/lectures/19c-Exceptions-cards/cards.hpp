//===========================================================================
//  A playing card class and related exception classes. 
//  Alice E. Fischer, March 2009                                cards.hpp
//===========================================================================
#pragma once
#include "bad.hpp"

enum SuitType{ spades, hearts, diamonds, clubs, bad };

//===========================================================================
// This is the main data class; it represents one playing card.
//
class  Card {
    int         spot_;
    SuitType    suit_;
public:
    // Card (istream& sin) throw (Bad, BadSpot, BadSuit);  // not c++17
    Card (istream& sin);
    int         spot(){ return spot_; }
    SuitType    suit(){ return suit_; }
    ostream&    print(ostream&);
    static void instructions(ostream&, int n);
};
