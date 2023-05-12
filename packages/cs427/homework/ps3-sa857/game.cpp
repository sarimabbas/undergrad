// ============================================================================
// Name: ThinkADot Game Class definitions                        File: game.cpp
// Author:  Sarim Abbas                                 Copyright: October 2018
// Modified October 2018
// ============================================================================
#include "game.hpp"
#include <ctype.h>

/**
 * @brief : runs an interactive prompt for user to play with ThinkADot
 *
 */
void Game::play( void ) {
    char key;
    cout << "Welcome to ThinkADot" << endl;
    do {
        // user input
        cout << "Input a key (input H for help): ";
        cin >> key;

        // validate input
        if (!checkInput( key )) {
            cout << "Invalid key, please try again." << endl;
            continue;
        }

        // check cases
        switch (toupper( key )) {
            case 'H':
                printHelpText();
                break;
            case 'P':
                cout << tad;
                break;
            case 'L':
                cout << "Resetting all flip-flops to 'L'..." << endl;
                tad.reset( 'L' );
                cout << tad;
                break;
            case 'R':
                cout << "Resetting all flip-flops to 'R'..." << endl;
                tad.reset( 'R' );
                cout << tad;
                break;
            case 'A':
                cout << "Dropping into A..." << endl;
                cout << "Came out of hole: " << tad.play( 'A' ) << endl;
                cout << tad;
                break;
            case 'B':
                cout << "Dropping into B..." << endl;
                cout << "Came out of hole: " << tad.play( 'B' ) << endl;
                cout << tad;
                break;
            case 'C':
                cout << "Dropping into C..." << endl;
                cout << "Came out of hole: " << tad.play( 'C' ) << endl;
                cout << tad;
                break;
        }
    } while(toupper(key) != 'Q');

    // exit
    cout << "Exiting game...";
}

/**
 * @brief : checks user input against the valid keys
 *
 * @param key : the user inputted key
 * @return true : if valid
 * @return false : if not valid
 */
bool Game::checkInput( char key ) const {
    bool check = false;
    for (int i = 0; i < VALID_KEY_COUNT; i++) {
        if (toupper(key) == valid_keys[i]) {
            check = true;
        }
    }
    return check;
}

/**
 * @brief : customizable helper function to print help text
 *
 */
void Game::printHelpText( void ) const {
    cout << "A: drop marble into left hole" << endl
    << "B: drop marble into middle hole" << endl
    << "C: drop marble into right hole" << endl
    << "L: reset ThinkADot to 'L' state" << endl
    << "R: reset ThinkADot to 'R' state" << endl
    << "P: print ThinkADot state" << endl
    << "H: request help text" << endl;
}
