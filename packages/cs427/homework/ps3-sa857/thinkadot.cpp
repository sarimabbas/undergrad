// ============================================================================
// Name: ThinkADot class definitions                        File: thinkadot.cpp
// Author:  Sarim Abbas                                 Copyright: October 2018
// Modified October 2018
// ============================================================================
#include "thinkadot.hpp"

/**
 * @brief Construct a new ThinkADot object
 *
 */
ThinkADot::ThinkADot() {
    for (int i = 0; i < FLIPFLOP_COUNT; i++) {
        flipFlops[i].setState( 'L' );
    }
}

/**
 * @brief : resets the ThinkADot
 *
 * @param key : can be either 'L' to reset to all 'L' or 'R' for all 'R'
 */
void ThinkADot::reset( char key ) {
    char reset_state;
    reset_state = (key == 'R') ? 'R' : 'L';

    for(int i = 0; i < FLIPFLOP_COUNT; i++) {
        flipFlops[i].setState( reset_state );
    }
}

/**
 * @brief : prints the current state of the ThinkADot
 *
 * @param out : stream to print to
 * @return ostream& : modified stream
 */
ostream& ThinkADot::print( ostream& out ) const {
    int top_bottom_count = (FLIPFLOP_COUNT / 2) - 1;
    int position = 0;

    // print top holes
    cout << "A " << "B " << "C" << endl;

    // print separator
    cout << "------" << endl;

    // print top row
    for (int i = 0; i < top_bottom_count; i++) {
        cout << flipFlops[i] << " ";
        position++;
    }
    cout << endl;

    // print middle row
    cout << " ";  // indent
    for (int i = position, j = position + top_bottom_count - 1; i < j; i++) {
        cout << flipFlops[i] << " ";
        position++;
    }
    cout << endl;

    // print bottom row
    for (int i = position, j = position + top_bottom_count; i < j; i++) {
        cout << flipFlops[i] << " ";
    }
    cout << endl;

    // print separator
    cout << "------" << endl;

    // print bottom holes
    cout << "P " << "  " << "Q" << endl;

    return cout;
}

/**
 * @brief : simulates a marble dropped through a top hole
 *
 * @param in_hole : the hole 'A', 'B' or 'C' the marble is dropped through
 * @return char : the hole 'P' or 'Q' the marble comes out through
 */
char ThinkADot::play( char in_hole ) {
    char out_hole;
    switch (in_hole) {
        case 'A':
            out_hole = recurse( 0 );
            break;
        case 'B':
            out_hole = recurse( 1 );
            break;
        case 'C':
            out_hole = recurse( 2 );
            break;
    }
    return out_hole;
}

/**
 * @brief : recursive helper function to simulate marble journey
 *
 * @param position : the index into the flipFlops array
 * @return char : the hole that the marble comes out of / 'P' or 'Q'
 */
char ThinkADot::recurse( int position ) {
    char ball_direction = flipFlops[position].flip();
    switch (position) {
        case 0:
            return (ball_direction == 'L') ? recurse( 5 ) : recurse( 3 );
            break;
        case 1:
            return (ball_direction == 'L') ? recurse( 3 ) : recurse( 4 );
            break;
        case 2:
            return (ball_direction == 'L') ? recurse( 4 ) : recurse( 7 );
            break;
        case 3:
            return (ball_direction == 'L') ? recurse( 5 ) : recurse( 6 );
            break;
        case 4:
            return (ball_direction == 'L') ? recurse( 6 ) : recurse( 7 );
            break;
        case 5:
            return 'P';
            break;
        case 6:
            return (ball_direction == 'L') ? 'P' : 'Q';
            break;
        case 7:
            return 'Q';
            break;
        default:
            // in case of error, return 'N'
            return 'N';
    }
}
