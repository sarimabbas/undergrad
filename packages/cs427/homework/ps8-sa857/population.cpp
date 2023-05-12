// ============================================================================
// Name: Population class declarations                  File: population.cpp
// Author:  Sarim Abbas                                 Copyright: November 2018
// Modified November 2018
// ============================================================================
#include "population.hpp"

/**
 * @brief Construct a new Population:: Population object
 * 
 * @param numAgents : no of agents in the population
 * @param probFickle : probability an agent is fickle
 * @param probOne : probability an agent has 1 as choice 
 */
Population::Population( int numAgents, double probNak, double probFickle, double probExtend ) {
    // initialize parameters
    Population::numAgents = numAgents;
    numNak = 0;
    numFickle = 0;
    numCrowd = 0;
    Population::probNak = probNak;
    Population::probFickle = probFickle;
    Population::probExtend = probExtend;

    // create fully dynamic array
    agents = new Agent*[numAgents];

    // create genesis block
    Block* genesis_block = new Block();

    // create genesis sptr
    SPtr genesis_sptr = SPtr( genesis_block );

    // create initial Blockchain
    // Blockchain init(genesis_sptr);

    // create population with probabilities
    for(int i = 0; i < Population::numAgents; i++) {
        // choose agent type
        double type_prob = dRandom();
        if (type_prob < probNak) {
            agents[i] = new Nakamoto( Blockchain( genesis_sptr ) );
            numNak++;
        } else if (type_prob < probNak + probFickle) {
            agents[i] = new Fickle( Blockchain( genesis_sptr ) );
            numFickle++;
        } else {
            agents[i] = new Crowd( Blockchain( genesis_sptr ) );
            numCrowd++;
        }
    }
}

/**
 * @brief : communicate between two agents in population
 * 
 * @param sender : an index in the agents array
 * @param receiver : an index in the agents array
 */
void Population::sendMessage(int sender, int receiver) {
    // get sender and receiver
    Agent* agt_sender = agents[sender];
    Agent* agt_receiver = agents[receiver];

    // keep receiver's choice saved
    Blockchain receiver_old_choice = agt_receiver->choice();

    // send message
    agt_receiver->update( agt_sender->choice() );

    // debug info
    cout << right << setw( 3 ) << "sendMessage(" << sender << ", " << receiver << ")" << endl;
}

void Population::extend( int extender ) {
    agents[extender]->extend();

    // debug info
    cout << right << setw( 3 ) << "extend(" << extender << ")" << endl;
    cout << right << setw( 3 ) <<  "New blockchain: ";
    agents[extender]->print( cout );
    cout << endl;
}

/**
 * @brief : return size of population
 * 
 * @return int 
 */
int Population::size() const {
    return numAgents;
}

/**
 * @brief : return size of population
 * 
 * @return int 
 */
Blockchain Population::agentChoice( int index ) const {
    return agents[index]->choice();
}

/**
 * @brief : return size of Fickle subpopulation
 * 
 * @return int 
 */
int Population::fickleSize() const {
    return numFickle;
}

/**
 * @brief : returrn size of Crowd subpopulation
 * 
 * @return int 
 */
int Population::crowdSize() const {
    return numCrowd;
}

/**
 * @brief : returrn size of Nakamoto subpopulation
 * 
 * @return int 
 */
int Population::nakSize() const {
    return numNak;
}

/**
 * @brief Destroy the Population:: Population object
 * 
 */
Population::~Population() {
    // free individual agents
    for(int i = 0; i < numAgents; i++) {
        delete agents[i];
    }

    // free array of pointers
    delete[] agents;
}

/**
 * @brief : print the population 
 * 
 * @param out : stream to print to
 * @return ostream& : modified stream
 */
ostream& Population::print(ostream& out) const {
    for(int i = 0; i < numAgents; i++) {
        out << "Agent " << i << " choice: ";
        agents[i]->print( out );
        out << endl;
    }
    return out;
}

/**
 * @brief : print the population statistics
 * 
 * @param out : stream to print to
 * @return ostream& : modified stream
 */
ostream& Population::printStats(ostream& out) const {
    out << right << setw( 13 ) << "numNak:" << right << setw( 7 )<< numNak << endl;
    out << right << setw( 13 ) << "numFickle:" << right << setw( 7 ) << numFickle << endl;
    out << right << setw( 13 ) << "numCrowd:" << right << setw( 7 ) << numCrowd << endl;
    out << right << setw( 13 ) << "probExtend:" << right << setw( 7 ) << probExtend << endl;
    return out;
}

/**
 * @brief : random number generation for fickle and choice selection
 * 
 * @return double 
 */
double Population::dRandom() { 
    return random() / ( RAND_MAX + 1.0 ); // result is double in [0,1) 
}
