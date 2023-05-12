// ============================================================================
// Name: Block class declarations                             File: block.hpp
// Author:  Sarim Abbas                                 Copyright: Dec 2018
// Modified Dec 2018
// ============================================================================
#pragma once
#include "SPtr.hpp"
#include "tools.hpp"

class Block {
    private:
        SPtr sptr; 
        const int serialNo = Serial::newID();
        const int level = 0;
    
        // delete copy constructor and copy assignment
        Block( const Block& ) = delete;
        Block& operator=( const Block& ) = delete;
   
    public:
        Block();            // for genesis/empty block
        Block( Block* dest ); // for existing block via raw ptr
        Block( const SPtr& sp ); // for existing block via sptr

        int blkLevel() const;
        int getID() const;
        ostream& print( ostream& out ) const;
        ostream& printChain( ostream& out ) const;
};
