// ============================================================================
// Name: Consensus Agent class definittions             File: agent.cpp
// Author:  Sarim Abbas                                 Copyright: October 2018
// Modified October 2018
// ============================================================================
#include "agent.hpp"

/**
 * @brief Construct a new Agent object
 * 
 * @param ch : the initial choice of the agent
 */
Agent::Agent( int ch ) {
    choice_register = ch;
}

/**
 * @brief : returns the agent's choice register
 * 
 * @return int : the agent's choice
 */
int Agent::choice() const {
    return choice_register;
}

/**
 * @brief : updates the agent's choice register
 * 
 * @param m : the choice to update with
 */
void Agent::update( int m ) {
    choice_register = m;
}

/**
 * @brief : print function for agent
 *
 * @param out : stream to print to
 * @return ostream& : modified stream
 */
ostream& Agent::print( ostream& out ) const {
    return out << "<Agent> Choice: " << choice() << endl;
}
