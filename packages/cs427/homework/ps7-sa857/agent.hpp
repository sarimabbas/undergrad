// ============================================================================
// Name: Consensus Agent class declarations             File: agent.hpp
// Author:  Sarim Abbas                                 Copyright: December 2018
// Modified December 2018
// ============================================================================
#pragma once
#include <string>
#include "blockchain.hpp"

class Agent {
    public:
        // virtual destructor
        virtual ~Agent(){};

        // returns the agent’s current choice
        virtual Blockchain choice() const = 0;

        // performs the update to the agent
        virtual void update( Blockchain m ) = 0;

        // extends blockchain of agent
        virtual void extend() = 0;

        // prints agent to stream
        virtual ostream& print( ostream& out ) const = 0;
};
