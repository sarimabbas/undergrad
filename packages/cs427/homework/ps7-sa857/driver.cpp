// ============================================================================
// Name: Driver class definitions                             File: driver.cpp
// Author:  Sarim Abbas                                 Copyright: Dec 2018
// Modified Dec 2018
// ============================================================================
#include "driver.hpp"

/**
 * @brief Construct a new Driver:: Driver object
 * 
 */
Driver::Driver() {
    cout << "Driver::Driver()" << endl;
    
    // create genesis block
    Block* genesis_block = new Block();

    // create genesis sptr
    SPtr genesis_sptr = SPtr( genesis_block );

    // construct Blockchains
    for (int i = 0; i < INITIAL_BLOCKCHAIN_COUNT; i++) {
        bc[i] = Blockchain( genesis_sptr );
    }
    
    // alternative:
    // manipulate sptr directly
    // advantage: less UIDs used
    // disadvantage: not good OOP design

    // alternative:
    // have Blockchain::Blockchain() such that it creates Blockchain with genesis block
    // assign to 9 other blockchains => sptr will be correctly incremented
    // disadvantage: confusion about the role of blockchain. Better to separate block and blockchain
}

/**
 * @brief : run the interactive prompt to control the driver
 * 
 */
void Driver::play( void ) {
    string command;
    cout << "Welcome to Blockchain Driver..." << endl;

    do {
        // user input
        cout << "Input a key (input H for help): ";
        cin >> command;

        // validate input
        if (!checkInput( command )) {
            cout << "Invalid key, please try again." << endl;
            continue;

        } else {
            // check cases
            switch (toupper( command[0] )) {

                case 'H': {
                    printHelpText(); 
                } break;

                case 'P': {
                    for (unsigned i = 0; i < INITIAL_BLOCKCHAIN_COUNT; i++) {
                        cout << i << " " << bc[i] << endl;
                    }
                } break;

                case 'A': {
                    int j = command[1] - '0';
                    int k = command[2] - '0';
                    
                    // simple statement
                    // because the copy assignment recurses
                    // through the private data members and invokes their copy assignment

                    // with a blockchain, the only data member is an sptr
                    // and its copy assignment operator is called

                    // in addition, the sptr copy assignment takes care of resetting the sptr
                    // (being assigned to)

                    bc[j] = bc[k];

                } break;

                case 'E': {
                    int j = command[1] - '0';

                    // on the right side
                    //      a block is dynamically allocated and points to current target
                    //      a blockchain is statically allocated and points to new block
                    //      blockchain is returned as r value
                    // on the left side
                    //      move assignment of right hand side to the left
                    //      existing sptr is move assigned
                    //              existing sptr is reset()
                    //              it takes over the properties
                    bc[j] = bc[j].extend();

                } break;

            }
        }
    } while (toupper( command[0] ) != 'Q');

    // exit
    cout << "Exiting driver...";
}

/**
 * @brief : check if specified command is valid
 * 
 * @param command : input by the user
 * @return true : if valid
 * @return false : if not
 */
bool Driver::checkInput( string command ) const {
    bool check = false;

    for (int i = 0; i < VALID_KEY_COUNT; i++) {
        if (toupper( command[0] ) == valid_keys[i]) {
            check = true;
        }
    }

    switch (command[0]) {
        case 'A':
            if (command.length() != 3) {
                check = false;
                break;
            }

            if (!isdigit( command[1] ) || !isdigit( command[2]) ) {
                check = false;
            }

            break;

        case 'E':
            if (command.length() != 2) {
                check = false;
                break;
            }

            if (!isdigit( command[1]) ) {
                check = false;
            }

            break;

        default:
            if(command.length() != 1) {
                check = false;
            }

            break;
    }

    return check;
}

/**
 * @brief : print help text for command H
 * 
 */
void Driver::printHelpText( void ) const {
    cout << "Ajk does the assignment bc[j] = bc[k]" << endl
         << "Ej extends blockchain bc[j]" << endl
         << "P prints the blockchains in array bc[], one per line" << endl
         << "Q quits" << endl
         << "H for help" << endl;
}