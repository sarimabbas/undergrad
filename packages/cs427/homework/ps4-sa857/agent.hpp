// ============================================================================
// Name: Consensus Agent class declarations             File: agent.hpp
// Author:  Sarim Abbas                                 Copyright: October 2018
// Modified October 2018
// ============================================================================
#pragma once

#include <string>
#include "tools.hpp"

class Agent {
    private:
        // choice register
        int choice_register;

    public:
        // constructor
        Agent() = delete;

        // custom constructor
        Agent( int ch );

        // returns the agent’s current choice
        int choice() const;

        // performs the update to the agent via fickle algorithm
        void update( int m );

        // prints agent to stream
        ostream& print( ostream& out ) const;
};

/**
 * @brief : extension of << to print Agent objects to stream
 *
 * @param out : stream to print to
 * @param a : Agent object
 * @return ostream& : modified stream
 */
inline ostream& operator<<( ostream& out, const Agent& a ) {
    return a.print( out );
}
