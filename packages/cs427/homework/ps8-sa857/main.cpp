// ============================================================================
// Name: Blockchain consensus program main                         File: main.cpp
// Author:  Sarim Abbas                                 Copyright: December 2018
// Modified December 2018
// ============================================================================
#include "simulator.hpp" 
#include "population.hpp"
#include "tools.hpp"
#include <string>

void run( int numAgents, double probNak, double probFickle, double probExtend, int seed, int maxRounds );
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
    int numAgents = stoi( argv[1] );
    int maxRounds = stoi ( argv[2] );
    double probNak = stod( argv[3] );
    double probFickle = stod( argv[4] );
    double probExtend = stod(argv[5]);

    // prepare seed
    int seed;
    if (argc > 6) { 
        seed = stoi( string( argv[6] ) );
    } else {
        seed = time( NULL );
    }
    srandom( seed );

    // run simulation
    run( numAgents, probNak, probFickle, probExtend, seed, maxRounds );

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
void run( int numAgents, double probNak, double probFickle, double probExtend, int seed, int maxRounds ) {
    // create a population
    Population pop( numAgents, probNak, probFickle, probExtend );

    // welcome output
    cout << "Welcome to Consensus Simulator!" << endl;
    cout << "Using seed: " << seed << endl;
    cout << right << setw( 13 ) << "numAgents:" << right << setw( 7 )<< numAgents << endl;
    cout << right << setw( 13 ) << "maxRounds:" << right << setw( 7 ) << maxRounds << endl;
    cout << right << setw( 13 ) << "probNak:" << right << setw( 7 ) << probNak << endl;
    cout << right << setw( 13 ) << "probFickle:" << right << setw( 7 ) << probFickle << endl;
    cout << right << setw( 13 ) << "probExtend:" << right << setw( 7 ) << probExtend << endl;
    cout << endl << "Population statistics:" << endl;
    pop.printStats(cout);
    cout << endl << "Starting simulation with probExtend=" << probExtend << endl;

    // run simulation
    Simulator sim(pop, maxRounds, probExtend);
    sim.run();

    // final output
    cout << "Current blockchain choices after " << maxRounds << " rounds:" << endl;
    pop.print(cout);
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
    if (argc < 6 || argc > 7) {
        cerr << "Usage: blockchain numAgents maxRounds probNak probFickle probExtend [seed]" << endl;
        return false;
    }

    // probCrowd = 1 - probNak - probFickle
    double probCrowd = 1 - stod( argv[3] ) - stod( argv[4] );
    if (probCrowd > 1.0 || probCrowd < 0.0) {
        cerr << "Usage: probNak + probFickle < 1.0" << endl;
        return false;
    }

    return true;
}
