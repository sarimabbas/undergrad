// ============================================================================
// Name: Consensus program main                         File: main.cpp
// Author:  Sarim Abbas                                 Copyright: November 2018
// Modified November 2018
// ============================================================================
#include "simulator.hpp" 
#include "population.hpp"
#include "tools.hpp"
#include <string>

void run( int numAgents, double probFickle, double probOne, int seed );
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

    banner();

    // prepare required args
    int numAgents = stoi( string( argv[1] ) );
    double probFickle = stod( argv[2] );
    double probOne = stod( argv[3] );

    // prepare seed
    int seed;
    if (argc > 4) { 
        seed = stoi( string( argv[4] ) );
    } else {
        seed = time( NULL );
    }
    srandom( seed );

    // run simulation
    run( numAgents, probFickle, probOne, seed );

    bye();

    return 0;
}

/**
 * @brief : runs the main logic of program
 * 
 * @param numAgents : the number of agents for simulation
 * @param numOne : how many agents are of choice 1
 * @param seed : seed for pseudo-random number generation
 */
void run( int numAgents, double probFickle, double probOne, int seed ) {
    // create a population
    Population pop( numAgents, probFickle, probOne );

    // create a simulator with population
    Simulator sim( pop );

    // run simulation
    sim.run();

    // collect results
    int rounds = sim.numRounds();
    int consensus = sim.consensusValue();
    int numFickle = pop.fickleSize();
    int numCrowd = pop.crowdSize();
    int numOnes = pop.oneSize();

    // data output
    cout << "Welcome to Consensus Simulator!" << endl;

    cout << "Using seed: " << seed << endl;
    cout << right << setw( 13 ) << "numAgents:" << right << setw( 7 )<< numAgents << endl;
    cout << right << setw( 13 ) << "probFickle:" << right << setw( 7 ) << probFickle << endl;
    cout << right << setw( 13 ) << "probOne:" << right << setw( 7 ) << probOne << endl;

    cout << "Population statistics:" << endl;
    cout << right << setw( 13 ) << "numFickle:" << right << setw( 7 ) << numFickle << endl;
    cout << right << setw( 13 ) << "numCrowd:" << right << setw( 7 ) << numCrowd << endl;
    cout << right << setw( 13 ) << "numOnes:" << right << setw( 7 ) << numOnes << endl;

    cout << "Results of simulation:" << endl;
    cout << right << setw( 13 ) << "Rounds:" << right << setw( 7 ) << rounds << endl;
    cout << right << setw( 13 ) << "Consensus:" << right << setw( 7 ) << consensus << endl;
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
    if (argc < 4 || argc > 5) {
        cerr << "Usage: consensus2 numAgents probFickle probOne [seed]" << endl;
        return false;
    }

    return true;
}
