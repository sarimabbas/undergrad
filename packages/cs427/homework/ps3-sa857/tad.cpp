//                  -*- mode:c++; tab-width:4 -*-
// File: tad.cpp ----------------------------------------------------------
// Runs a ThinkADot game
// Author: Sarim Abbas.
// Created September 2018. Modified September 2018.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "game.hpp"

void run( void );

// ---------------------------------------------------------------------------
//  Calls functions to show a bannner, start a ThinkADot game, and exit.
int main(int argc, char* argv[]) {
    banner();
    run();
    bye();
}

// ---------------------------------------------------------------------------
//  Create a ThinkADot Game object and start a game
void run( void ) {
    Game tadGame;
    tadGame.play();
}
