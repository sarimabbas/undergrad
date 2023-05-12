// ============================================================================
// Name: Blockchain class definitions                             File: blockchain.cpp
// Author:  Sarim Abbas                                 Copyright: Dec 2018
// Modified Dec 2018
// ============================================================================
#include "blockchain.hpp"

/**
 * @brief Construct a new Blockchain:: Blockchain object
 * 
 * @param sp : construct with another sptr (to increment count)
 */
Blockchain::Blockchain( const SPtr& sp ) : sptr( sp ) {
    // cout << "Blockchain::Blockchain(const SPtr& sp)" << endl;
}

/**
 * @brief Construct a new Blockchain:: Blockchain object
 * 
 * @param dest : construct with raw Block* (does not increment count)
 */
Blockchain::Blockchain( Block* dest ) : sptr( dest ) {
    // cout << "Blockchain::Blockchain( Block* dest ) : sptr( dest )" << endl;
    // dest is a ptr to some dynamically allocated Block
}

/**
 * @brief Construct a new Blockchain:: Blockchain object
 * 
 */
Blockchain::Blockchain() {
    // cout << "Blockchain::Blockchain()" << endl;
}

/**
 * @brief : returns a new blockchain created by extending the current
 * blockchain
 *
 * @return Blockchain
 */
Blockchain Blockchain::extend() {
    // new block initializes its sptr with sptr of curr blockchain
    // this allows for incrementing the counts
    Block* new_block_ptr = new Block( sptr );

    // create new blockchain
    // it will be constructed with the newly created block 
    // this will set up a new sptr with count == 0
    // an alternative is to move in a temp sptr using: new_blockchain ( SPtr( new_block_ptr ) )
    Blockchain new_blockchain( new_block_ptr );

    return new_blockchain;
}

/**
 * @brief : returns raw Block* ptr to tail of blockchain
 * 
 * @return Block* 
 */
Block* Blockchain::tail() const {
    return sptr.getTarget(); 
}

/**
 * @brief : return length of chain as blkLevel + 1
 * 
 * @return unsigned : length
 */
unsigned Blockchain::length() const {
    // total length = current level + 1
    return sptr->blkLevel() + 1;
}

/**
 * @brief : prints block chain 
 * 
 * @param out : stream to modify
 * @return ostream& : the modified stream
 */
ostream& Blockchain::print( ostream& out ) const {
    if (!sptr.empty()) {
        return sptr->printChain( out );
    }
    cout << "Blockchain::print() => Empty chain" << endl;
    return out;
}