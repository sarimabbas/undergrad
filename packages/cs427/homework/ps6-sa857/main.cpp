// ============================================================================
// Name: Blockchain program main                             File: main.cpp
// Author:  Sarim Abbas                                 Copyright: Dec 2018
// Modified Dec 2018
// ============================================================================
#include "driver.hpp"

void run( void );

/**
 * @brief : entry point for blockchain executable
 *
 * @param argc : argument count
 * @param argv : argument array
 * @return int : exit code
 */
int main( int argc, char** argv ) {
    banner();
    run();
    bye();
    return 0;
}

/**
 * @brief : instantiates a driver and runs the interactive loop
 * 
 */
void run( void ) {
    Driver d;
    d.play();
}
