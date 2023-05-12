//                  -*- mode:c++; tab-width:4 -*-
// File: test.cpp ----------------------------------------------------------
// Implementation file for testing FlipFlop, ThinkADot and Game classes
// Author: Sarim Abbas
// Created September 2018. Modified September 2018.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "tools.hpp"
#include "flipflop.hpp"
#include "thinkadot.hpp"
#include "game.hpp"

int main(int argc, char* argv[]) {
    banner();

    // test functionality of flip flop
    FlipFlop ff;
    cout << ff.flip() << endl;
    ff.setState( 'R' );
    cout << ff.getState() << endl;

    // test functionality of Think A Dot
    ThinkADot tad;
    cout << tad;
    tad.play( 0 );
    cout << tad;
    tad.reset( 'L' );
    cout << tad;

    // test functionality of Game
    Game tadgame;
    tadgame.play();
    
    bye();
}
