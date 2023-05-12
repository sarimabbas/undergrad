// ============================================================================
// Name: Crowd Agent class definitionns                 File: crowd.cpp
// Author:  Sarim Abbas                                 Copyright: November 2018
// Modified November 2018
// ============================================================================
#include "crowd.hpp"

/**
 * @brief Construct a new Crowd object 
 * 
 * @param ch : choice to initialize Crowd agent with
 */
Crowd::Crowd( int ch ) {
    last_message = ch;
    choice_register = ch;
}

/**
 * @brief : returns the agent's choice register
 * 
 * @return int : the agent's choice
 */
int Crowd::choice() const {
    return choice_register;
}

/**
 * @brief : updates the agent's choice register
 * 
 * @param m : the choice to update with
 */
void Crowd::update( int m ) {
    // update the choice only if the current message
    // is equal to the last message received
    if (m == last_message) {
        choice_register = m;
    }

    // update the last message
    last_message = m;
}

/**
 * @brief : print function for agent
 *
 * @param out : stream to print to
 * @return ostream& : modified stream
 */
ostream& Crowd::print( ostream& out ) const {
    return out << "<Crowd> Choice: " << choice() << endl;
}
