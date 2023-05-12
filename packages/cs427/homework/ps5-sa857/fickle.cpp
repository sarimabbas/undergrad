// ============================================================================
// Name: Fickle Agent class definitionns                File: fickle.cpp
// Author:  Sarim Abbas                                 Copyright: November 2018
// Modified November 2018
// ============================================================================
#include "fickle.hpp"

/**
 * @brief Construct a new Fickle:: Fickle object 
 * 
 * @param ch : choice to initialize Fickle agent with
 */
Fickle::Fickle( int ch ) {
    choice_register = ch;
}

/**
 * @brief : returns the agent's choice register
 * 
 * @return int : the agent's choice
 */
int Fickle::choice() const {
    return choice_register;
}

/**
 * @brief : updates the agent's choice register
 * 
 * @param m : the choice to update with
 */
void Fickle::update( int m ) {
    choice_register = m;
}

/**
 * @brief : print function for agent
 *
 * @param out : stream to print to
 * @return ostream& : modified stream
 */
ostream& Fickle::print( ostream& out ) const {
    return out << "<Fickle> Choice: " << choice() << endl;
}
