// ============================================================================
// Name: Agent Base class definitionns                File: agentbase.cpp
// Author:  Sarim Abbas                                 Copyright: November 2018
// Modified November 2018
// ============================================================================
#include "agentbase.hpp"

/**
 * @brief Construct a new AgentBase:: AgentBase object 
 * 
 * @param ch : choice to initialize AgentBase agent with
 */
AgentBase::AgentBase( Blockchain ch ) {
    choice_register = ch;
}

/**
 * @brief : returns the agent's choice register
 * 
 * @return int : the agent's choice
 */
Blockchain AgentBase::choice() const {
    return choice_register;
}

/**
 * @brief : extend the agent's blockchain
 * 
 */
void AgentBase::extend() {
    choice_register = choice_register.extend();
}

ostream& AgentBase::print( ostream& out ) const {
    choice_register.print( out );
    return out;
}