// ============================================================================
// Name: Block class definitions                             File: block.cpp
// Author:  Sarim Abbas                                 Copyright: Dec 2018
// Modified Dec 2018
// ============================================================================
#include "block.hpp" 

/**
 * @brief Construct a new Block:: Block object.
 * Meant for creating genesis blocks
 * Its sptr is empty and points to nothing
 * Its level is 0
 * 
 */
Block::Block() : level( 0 ) {
    // initialize sptr with nothing (empty)
    // cout << "Block::Block() : level( 0 )" << endl;
}

/**
 * @brief Construct a new Block:: Block object
 * Its sptr is initialized with raw ptr
 * Its level is initialized via existing block
 * 
 * @param dest : construct with raw ptr, which does not increment count
 */
Block::Block( Block* dest ) : sptr( dest ), level( dest->blkLevel() + 1 ) {
    // cout << "Block::Block(Block* dest) : sptr(dest)" << endl;
}

/**
 * @brief Construct a new Block:: Block object
 * Its sptr is initialized with existing sptr
 * Its level is initialized with existing block
 * 
 * @param sp : construct with an existing sptr, which increments count
 */
Block::Block( const SPtr& sp ) : sptr( sp ), level( sptr->blkLevel() + 1 ) {
    // cout << "Block::Block(const SPtr& sp) : sptr(sp)" << endl;
}


/**
 * @brief : return level of block
 * 
 * @return int 
 */
int Block::blkLevel() const {
    return level;
}

/**
 * @brief : return UID of block
 * 
 * @return int 
 */
int Block::getID() const {
    return serialNo;
}

/**
 * @brief : print block to stream 
 * 
 * @param out : stream to modify
 * @return ostream& : modified stream
 */
ostream& Block::print( ostream& out ) const {
    out << "[" << level << "," << serialNo << "]";
    return out;
}

/**
 * @brief : print chain of blocks
 * 
 * @param out : stream to modify
 * @return ostream& : modified stream
 */
ostream& Block::printChain( ostream& out ) const {
    // print the next block first if not empty
    if (!sptr.empty()) {
        sptr->printChain( out );
    }
    // then print the current
    return print( out );
}
