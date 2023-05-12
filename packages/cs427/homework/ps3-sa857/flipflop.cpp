// ============================================================================
// Name: ThinkADot FlipFlop class definitions                File: flipflop.cpp
// Author:  Sarim Abbas                                 Copyright: October 2018
// Modified October 2018
// ============================================================================
#include "flipflop.hpp"

/**
 * @brief : Flips state from LEFT to RIGHT and vice versa
 *
 * @return char : Returns the char representation of state the ball is on when
 * leaving
 */
char FlipFlop::flip( void ) {
    // if state is NONE, return 'N'
    if (FlipFlop::state == FlipFlop::NONE) { return 'N'; }
   
    // find which way the ball will go
    // if LEFT/right-leaning then ball will go 'L'
    // if RIGHT/left-leaning thenn ball will go 'R'
    char ball_direction = getState();

    // flip the flip flop
    state = (state == FlipFlop::LEFT) ? FlipFlop::RIGHT : FlipFlop::LEFT;

    // return the char representation of which way the ball will go
    return ball_direction;
}

/**
 * @brief : get the state data member as a char representation
 *
 * @return char : the char representation of the state data member
 */
char FlipFlop::getState( void ) const {
    // if state is NONE, return 'N'
    if (state == FlipFlop::NONE) { return 'N'; }

    // if the state is LEFT/right-leaning, return 'L'
    char letter_state = (state == FlipFlop::LEFT) ? 'L' : 'R';

    // return the char representation of the state
    return letter_state; 
}

/**
 * @brief : set the state data member
 *
 * @param input : what to set the state to (a char representation)
 */
void FlipFlop::setState( char input ) { 
    // if LEFT/right-leaning set the state to LEFT
    state = (input == 'L') ? FlipFlop::LEFT : FlipFlop::RIGHT;
}

/**
 * @brief : print function
 *
 * @param out : stream to print to
 * @return ostream& : modified stream
 */
ostream& FlipFlop::print( ostream& out ) const {
    char c;
    // return character based on state
    switch (state) {
        case LEFT:
            c = 'L';
            break;
        case RIGHT:
            c = 'R';
            break;
        default:
            c = 'N';
    }
    return out << c;
}
