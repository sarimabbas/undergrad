// ============================================================================
// Name: Agent Base class declarations                File: agentbase.hpp
// Author:  Sarim Abbas                                 Copyright: November 2018
// Modified November 2018
// ============================================================================
#pragma once
#include "agent.hpp"

class AgentBase: public Agent {
    protected:
        // choice register
        Blockchain choice_register;
    public:
        // constructor
        AgentBase() = delete;

        // custom constructor
        AgentBase( Blockchain ch );

        // extend blockchain
        void extend();

        // returns the agent’s current choice
        Blockchain choice() const;

        // print
        ostream& print( ostream& out ) const;
};

/**
 * @brief : extend << operator for agent
 * 
 * @param out : stream to print to
 * @param c : reference to crowd object
 * @return ostream& : modified stream
 */
inline ostream& operator<<( ostream& out, AgentBase& ab ) {
    return ab.print( out );
}
