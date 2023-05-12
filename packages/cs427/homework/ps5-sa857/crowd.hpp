// ============================================================================
// Name: Crowd Agent class declarations                 File: crowd.hpp
// Author:  Sarim Abbas                                 Copyright: November 2018
// Modified November 2018
// ============================================================================
#pragma once
#include "agent.hpp"

class Crowd: public Agent {
    private:
        // choice register
        int choice_register;

        // last choice
        int last_message;
    public:
        // constructor
        Crowd() = delete;

        // custom constructor
        Crowd( int ch );

        // returns the agent’s current choice
        int choice() const;

        // performs the update to the agent via fickle algorithm
        void update( int m );

        // prints agent to stream
        ostream& print( ostream& out ) const;
}; 

/**
 * @brief : extend << operator for crowd agent
 * 
 * @param out : stream to print to
 * @param c : reference to crowd object
 * @return ostream& : modified stream
 */
inline ostream& operator<<( ostream& out, const Crowd& c ) {
    return c.print( out );
}
