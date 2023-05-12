// ============================================================================
// Name: Consensus program main                         File: main.cpp
// Author:  Sarim Abbas                                 Copyright: October 2018
// Modified October 2018
// ============================================================================
#include "agent.hpp"
#include "simulator.hpp" 
#include "tools.hpp"
#include <iostream>
#include <string>

void run( int no_agents, int no_agents_one, int seed );
bool claParse( int argc, char** argv );

/**
 * @brief : entry point for consensus executable
 * 
 * @param argc : argument count
 * @param argv : argument array
 * @return int : exit code
 */
int main( int argc, char** argv ) {
    // parse arguments
    if (!claParse( argc, argv )) { return 1; }

    // banner();

    // prepare required args
    int numAgents = stoi( string( argv[1] ) );
    int numOne = stoi( string( argv[2] ) );

    // prepare seed
    int seed;
    if (argc > 3) { 
        seed = stoi( string( argv[3] ) );
    } else { 
        seed = time( NULL );
    }

    // run simulation
    run( numAgents, numOne, seed );

    // bye();

    // exit
    return 0;
}

/**
 * @brief : runs the main logic of program
 * 
 * @param numAgents : the number of agents for simulation
 * @param numOne : how many agents are of choice 1
 * @param seed : seed for pseudo-random number generation
 */
void run( int numAgents, int numOne, int seed ) {
    // create a simulator
    Simulator sim( numAgents, numOne, seed );

    // create a rounds integer and run simulation
    int rounds;
    int consensus = sim.run( rounds );

    // The number of agents, the number of agents initially choosing one, the
    // actual seed used, the number of communication rounds required to reach
    // consensus, and the ﬁnal consensus value
    cout << numAgents << " " << numOne << " " << seed << " " << rounds << " " << consensus << endl;
}

/**
 * @brief : parses command line arguments
 * 
 * @param argc : argument count
 * @param argv : argument array
 * @return true : if valid
 * @return false : if invalid
 */
bool claParse( int argc, char** argv ) {
    if (argc < 3 || argc > 4) {
        cerr << "Usage: consensus numAgents numOne [seed]" << endl;
        return false;
    }

    if (atoi( argv[1] ) < atoi( argv[2] )) {
        cerr << "Usage: numOne <= numAgents" << endl;
        return false;
    }

    return true;
}
