// ============================================================================
// Name: ThinkADot Game class declarations                       File: game.hpp
// Author:  Sarim Abbas                                 Copyright: October 2018
// Modified October 2018
// ============================================================================
#ifndef GAME_HPP
#define GAME_HPP
#define VALID_KEY_COUNT 8

#include "tools.hpp"
#include "game.hpp"
#include "thinkadot.hpp"


class Game {
    private:
        /**
         * @brief : stack-allocated ThinkADot data member
         * 
         */
        ThinkADot tad;

        /**
         * @brief : valid keys to interact with the ThinkADot
         * 
         */
        char valid_keys[VALID_KEY_COUNT] = {'L', 'R', 'A', 'B', 'C', 'Q', 'P', 'H'};

        bool checkInput( char key ) const;

        void printHelpText( void ) const;

    public:

        void play( void );
};

#endif
