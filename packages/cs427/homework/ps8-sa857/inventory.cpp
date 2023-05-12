// ============================================================================
// Name: Inventory class definitions                      File: inventory.hpp
// Author:  Sarim Abbas                                 Copyright: Dec 2018
// Modified Dec 2018
// ============================================================================
#include "inventory.hpp"

Inventory::Inventory() {}

/**
 * @brief : add a blockchain to the inventory, updating count or making new
 * entry as needed
 * 
 * @param bc : blockchain to add
 */
void Inventory::add( Blockchain bc ) {
    // cout << "Adding: " << bc << endl;

    blockchainMap::iterator pos;
    pos = inventory.find( bc );

    // found
    if (pos != inventory.end() ) {
        pos->second++;
    // not found
    } else {
        // cout << "Not found, inserting: " << bc << endl;
        inventory[bc] = 1;
    }
}

/**
 * @brief : subtract a blockchain from the inventory, updating count or erasing
 * entry as needed
 *
 * @param bc : blockchain to sub
 */
void Inventory::sub( Blockchain bc ) {
    // cout << "Subbing: " << bc << endl;

    blockchainMap::iterator pos;
    pos = inventory.find(bc);

    // found
    if (pos != inventory.end()) {
        // decrement count
        pos->second--;
        // delete if drops to 0
        if (pos->second == 0) {
            inventory.erase( pos );
        }
    }
}

/**
 * @brief : print inventory to stream
 * 
 * @param out : stream to modify
 * @return ostream& : modified stream
 */
ostream& Inventory::print( ostream& out ) {
    for (blockchainMap::iterator pos=inventory.begin(); pos!=inventory.end(); pos++) {
        out << right << setw( 3 ) << pos->second << " copies of: ";
        pos->first.print(out);
        out << endl;
    }
    return out;
}

/**
 * @brief : return size of inventory
 * 
 * @return int : size
 */
int Inventory::size() const {
    return inventory.size();
}
