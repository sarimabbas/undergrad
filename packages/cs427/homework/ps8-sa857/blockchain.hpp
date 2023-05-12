// ============================================================================
// Name: Block class declarations                             File: blockchain.hpp
// Author:  Sarim Abbas                                 Copyright: Dec 2018
// Modified Dec 2018
// ============================================================================
#pragma once
#include "block.hpp" 
#include <string>
#include <vector>

class Blockchain {
    private:
        SPtr sptr;

    public:
        Blockchain extend();

        // return length of blockchain
        unsigned length() const;

        // return raw ptr to the tail block
        Block* tail() const;
        
        // print blockchain
        ostream& print( ostream& out ) const;

        // test two blockchains for equality
        bool operator==( const Blockchain& other ) const {
            // two blockchains are equal if they have the same tail
            if (tail()->getID() == other.tail()->getID()) {
                return true;
            } else {
                return false;
            }
        }

        bool operator<( const Blockchain& other ) const {
            // one blockchain is less than the other based on ID
            if(length() == other.length()) {
                if (tail()->getID() > other.tail()->getID()) {
                    return true;
                } else {
                    return false;
                }
            } else if(length() > other.length()) {
                return true;
            } else {
                return false;
            }
        }

        // default constructor
        Blockchain();

        // creates Blockchain object
        // its sptr is initialized with a raw Block ptr
        Blockchain( Block* dest ); 

        // creates Blockchain object
        // its sptr is initialized with another sptr => increments count
        Blockchain( const SPtr& sp );
};

/**
 * @brief : extend << operator to print a blockchain
 * 
 * @param out : stream to modify
 * @param bc : block chain to print
 * @return ostream& : modified stream
 */
inline ostream& operator<<( ostream& out, Blockchain& bc ) {
    return bc.print( out );
}

