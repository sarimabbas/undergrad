// ============================================================================
// Name: Nakamoto, Fickle and Crowd class definitionns      File: multiagent.cpp
// Author:  Sarim Abbas                                 Copyright: November 2018
// Modified November 2018
// ============================================================================
#include "multiagent.hpp"

/**
 * @brief Construct a new Nakamoto:: Nakamoto object 
 * 
 * @param ch : choice to initialize Nakamoto agent with
 */
Nakamoto::Nakamoto( Blockchain ch ) : AgentBase( ch ) {}

/**
 * @brief : updates the agent's choice register
 * 
 * @param m : the choice to update with
 */
void Nakamoto::update( Blockchain m ) {
    if (m.length() > choice_register.length()) {
        choice_register = m;
    }
}

/**
 * @brief Construct a new Crowd object 
 * 
 * @param ch : choice to initialize Crowd agent with
 */
Crowd::Crowd( Blockchain ch ) : AgentBase( ch ) {
    last_message = ch;
}

/**
 * @brief : updates the agent's choice register
 * 
 * @param m : the choice to update with
 */
void Crowd::update( Blockchain m ) {
    // update the choice only if the current message
    // is equal to the last message received
    if (m == last_message) {
        choice_register = m;
    }

    // update the last message
    last_message = m;
}

/**
 * @brief Construct a new Fickle:: Fickle object 
 * 
 * @param ch : choice to initialize Fickle agent with
 */
Fickle::Fickle( Blockchain ch ) : AgentBase( ch ) { }

/**
 * @brief : updates the agent's choice register
 * 
 * @param m : the choice to update with
 */
void Fickle::update( Blockchain m ) {
    choice_register = m;
}
