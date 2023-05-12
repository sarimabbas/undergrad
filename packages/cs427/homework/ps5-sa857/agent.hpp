// ============================================================================
// Name: Consensus Agent class declarations             File: agent.hpp
// Author:  Sarim Abbas                                 Copyright: November 2018
// Modified November 2018
// ============================================================================
#pragma once
#include <string>
#include "tools.hpp"

class Agent {
    public:
        // virtual destructor
        virtual ~Agent(){};

        // returns the agent’s current choice
        virtual int choice() const = 0;

        // performs the update to the agent
        virtual void update( int m ) = 0;

        // prints agent to stream
        virtual ostream& print( ostream& out ) const = 0;
};
