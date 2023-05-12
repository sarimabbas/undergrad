// ============================================================================
// Name: Consensus Simulator class definitions          File: simulator.cpp
// Author:  Sarim Abbas                                 Copyright: October 2018
// Modified October 2018
// ============================================================================
#include "simulator.hpp"

/**
 * @brief Construct a new Simulator object
 * 
 * @param numAgents : number of agents in agents array
 * @param numOne : how many agents have choice 1
 * @param seed : seed for pseudo-random number generation
 */
Simulator::Simulator( int numAgents, int numOne, unsigned int seed ) {
    // init counts
    agentOnes = 0; 
    agentZeros = 0;

    // init agents with 1
    for(int i = 0; i < numOne; i++) { 
        agents.push_back( Agent( 1 ) ); 
        agentOnes++; 
    }

    // init agents with 0
    for(int i = numOne; i < numAgents; i++) { 
        agents.push_back( Agent( 0 ) ); 
        agentZeros++; 
    }
    
    // set seed
    srandom( seed );
}

/**
 * @brief : runs the simulation
 * 
 * @param rounds : gets updated with rounds elapsed
 * @return int : the consensus value
 */
int Simulator::run( int& rounds ) {
    // rounds elapsed
    rounds = 0;

    for(;;) {
        // break if consensus is reached
        if (agentOnes == 0 || agentZeros == 0) {
            break;
        }

        // select sender
        int total = agentOnes + agentZeros;
        int j_pos = uRandom( total );
        Agent& j = agents[j_pos];

        // select receiver
        int k_pos = uRandom( total - 1 );
        if (k_pos >= j_pos) { k_pos++; }
        Agent& k = agents[k_pos];
        
        // set up a commmunication between them
        communicate( j, k );

        // update rounds
        rounds++;
    }

    // return consensus value
    int consensus = agentOnes == 0 ? 0 : 1;
    return consensus;
}

/**
 * @brief : communicate between two agents and update simulator counts
 *
 * @param j : the sender agent
 * @param k : the receiver agent
 */
void Simulator::communicate( Agent& j, Agent& k ) {
    // keep k's choice saved
    int k_old_choice = k.choice();

    // send message from j to k
    int msg = j.choice();
    k.update( msg );

    // check if k's choice changed, and update accordingly
    int k_new_choice = k.choice();
    if (k_new_choice != k_old_choice) {
        if (k_new_choice == 0) {
            agentZeros++;
            agentOnes--;
        } else {
            agentOnes++;
            agentZeros--;
        }
    }
}

/**
 * @brief : print function
 *
 * @param out : stream to print to
 * @return ostream& : modified stream
 */
ostream& Simulator::print( ostream& out ) const {
    out << "# Choice 0: " << agentZeros << " ";
    out << "# Choice 1: " << agentOnes << endl;
    for(int i = 0; i < agentOnes + agentZeros; i++) {
        out << "POS " << i << " " << agents[i];
    }
    return out;
}

/**
 * @brief : generates random number from [0, n - 1]
 * 
 * @param n : the number in above expression
 * @return int : the random number
 */
int uRandom( int n ) {
    long int usefulMax = RAND_MAX - (( RAND_MAX - n ) + 1) % n;  // avoids int overflow
    long int r;
    do { r = random(); } 
    while (r > usefulMax);
    return r % n;
}
