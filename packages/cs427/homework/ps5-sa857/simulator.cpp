// ============================================================================
// Name: Consensus Simulator class definitions          File: simulator.cpp
// Author:  Sarim Abbas                                 Copyright: November 2018
// Modified November 2018
// ============================================================================
#include "simulator.hpp"

/**
 * @brief Construct a new Simulator:: Simulator object
 * 
 * @param pop : ctor for referenced population
 */
Simulator::Simulator( Population& pop ) : pop( pop ) {
    rounds = 0;
}

/**
 * @brief : run simulation on the referenced population
 * 
 */
void Simulator::run() {
    for(;;) {

        // break if consensus is reached
        if (pop.consensusReached()) {
            break;
        }

        // pick two random agents
        int total = pop.size();
        int sender = uRandom( total );
        int receiver = uRandom( total - 1 );
        if (receiver >= sender) { receiver++; }

        // communicate
        pop.sendMessage( sender, receiver );

        // update rounds
        rounds++;
    }
}

/**
 * @brief : return the number of rounds of simulation elapsed
 * 
 * @return int 
 */
int Simulator::numRounds() {
    return rounds;
}

/**
 * @brief : return the consensus value
 * 
 * @return int 
 */
int Simulator::consensusValue() {
    return pop.consensusValue();
}

/**
 * @brief : generates random number from [0, n - 1]
 * 
 * @param n : the number in above expression
 * @return int : the random number
 */
int Simulator::uRandom( int n ) {
    long int usefulMax = RAND_MAX - (( RAND_MAX - n ) + 1) % n;  // avoids int overflow
    long int r;
    do { r = random(); } 
    while (r > usefulMax);
    return r % n;
}
