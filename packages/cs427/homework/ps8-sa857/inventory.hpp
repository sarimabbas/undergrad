// ============================================================================
// Name: Driver class declarations                             File: driver.hpp
// Author:  Sarim Abbas                                 Copyright: Dec 2018
// Modified Dec 2018
// ============================================================================
#pragma once
#include "blockchain.hpp"
// ============================================================================
// Name: Inventory class declarations                       File: inventory.hpp
// Author:  Sarim Abbas                                 Copyright: Dec 2018
// Modified Dec 2018
// ============================================================================
#include <map>
#include <string>
#include <iterator>

typedef map<Blockchain, int> blockchainMap;

class Inventory {

    private:
        // array of blockchains
        blockchainMap inventory;

    public:
        // add to inventory
        void add( Blockchain bc );

        // remove from inventory
        void sub( Blockchain bc );
        
        // print inventory
        ostream& print( ostream& out );

        // size of inventory
        int size() const;

        // constructor
        Inventory();
};
