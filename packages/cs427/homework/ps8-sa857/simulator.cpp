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
Simulator::Simulator( Population& pop, int maxRounds, double probExtend ) : pop( pop ) {
    Simulator::maxRounds = maxRounds;
    Simulator::probExtend = probExtend;

    // initialize inventory
    int len = pop.size();
    for(int i = 0; i < len; i++) {
        inventory.add( pop.agentChoice( i ) );
    }

    // debug info
    cout << "Inventory: " << inventory.size() << " active blockchain(s):" << endl;
    printInventory( cout );
}

/**
 * @brief : run simulation on the referenced population
 * 
 */
void Simulator::run() {
    for(int i = 0; i < maxRounds; i++) {
        
        int total = pop.size();
        double op_prob = dRandom();

        // extend
        if (op_prob < probExtend) {

            int extender = uRandom(total);

            // store old choice
            Blockchain old_choice = pop.agentChoice( extender );
            
            // extend
            pop.extend( extender );

            // retrieve new choice
            Blockchain new_choice = pop.agentChoice( extender );

            // if changed, update counts
            if (!(old_choice == new_choice)) {
                inventory.sub( old_choice );
                inventory.add( new_choice );

                // print debug info
                cout << "Inventory: " << inventory.size() << " active blockchain(s):" << endl;
                printInventory( cout );
            }

        // update
        } else {
            // pick two random agents
            int sender = uRandom( total );
            int receiver = uRandom( total - 1 );
            if (receiver >= sender) { receiver++; }

            // store old choice
            Blockchain old_choice = pop.agentChoice( receiver );

            // communicate
            pop.sendMessage(sender, receiver);

            // retrieve new choice
            Blockchain new_choice = pop.agentChoice( receiver );

            // if changed, update counts
            if (!(old_choice == new_choice)) {
                inventory.sub( old_choice );
                inventory.add( new_choice );

                // print debug info
                cout << "Inventory: " << inventory.size() << " active blockchain(s):" << endl;
                printInventory( cout );
            }
        }
    }
}

/**
 * @brief : return the number of rounds of simulation
 * 
 * @return int 
 */
int Simulator::rounds() const {
    return maxRounds;
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

/**
 * @brief : random number generation for operation selection
 * 
 * @return double 
 */
double Simulator::dRandom() { 
    return random() / ( RAND_MAX + 1.0 ); // result is double in [0,1) 
}

ostream& Simulator::printInventory(ostream& out) {
    inventory.print( out );
    return out;
}