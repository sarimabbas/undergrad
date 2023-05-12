// ============================================================================
// Name: Fickle Agent class declarations                File: fickle.hpp
// Author:  Sarim Abbas                                 Copyright: November 2018
// Modified November 2018
// ============================================================================
#pragma once
#include "agent.hpp"

class Fickle: public Agent {
    private:
        // choice register
        int choice_register;
    public:
        // constructor
        Fickle() = delete;

        // custom constructor
        Fickle( int ch );

        // returns the agent’s current choice
        int choice() const;

        // performs the update to the agent via fickle algorithm
        void update( int m );

        // prints agent to stream
        ostream& print( ostream& out ) const;
};

/**
 * @brief : extend << operator for fickle agent
 *
 * @param out : stream to print to
 * @param f : reference to fickle object
 * @return ostream& : modified stream
 */
inline ostream& operator<<( ostream& out, const Fickle& f ) {
    return f.print( out );
}
