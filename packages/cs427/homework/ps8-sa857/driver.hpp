// ============================================================================
// Name: Driver class declarations                             File: driver.hpp
// Author:  Sarim Abbas                                 Copyright: Dec 2018
// Modified Dec 2018
// ============================================================================
#pragma once
#define VALID_KEY_COUNT 5
#define INITIAL_BLOCKCHAIN_COUNT 10

#include "blockchain.hpp"
#include <ctype.h>
#include <vector>
#include <string>

class Driver {

    private:
        // array of blockchains
        Blockchain bc[INITIAL_BLOCKCHAIN_COUNT];

        // the valid keys for the driver
        char valid_keys[VALID_KEY_COUNT] = {'A', 'E', 'P', 'Q', 'H'};

        // check the command input by the user
        bool checkInput( string key ) const;

        // print help text
        void printHelpText(void) const;

    public:
        // run interactive prompt
        void play( void );

        // constructor
        Driver();
};