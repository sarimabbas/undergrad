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
Population::Population( int numAgents, double probFickle, double probOne ) {
    // initialize parameters
    Population::numAgents = numAgents;
    numOnes = 0;
    numFickle = 0;
    zeroTracker = 0;

    // create fully dynamic array
    agents = new Agent*[numAgents];

    // create population with probabilities
    for(int i = 0; i < Population::numAgents; i++) {

        // choose choice
        int choice;
        double choice_prob = dRandom();
        if (choice_prob < probOne) {  // TODO: <= ???
            choice = 1;
            numOnes++;
        } else {
            zeroTracker++;
            choice = 0;
        }

        // choose agent type
        double type_prob = dRandom();
        if (type_prob < probFickle) {
            agents[i] = new Fickle( choice );
            numFickle++;
        } else {
            agents[i] = new Crowd( choice );
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
    int receiver_old_choice = agt_receiver->choice();

    // send message
    int msg = agt_sender->choice();
    agt_receiver->update( msg );

    // check if choice changed
    int receiver_new_choice = agt_receiver->choice();

    // update counts accordingly
    if (receiver_new_choice != receiver_old_choice) {
        if (receiver_new_choice == 0) {
            zeroTracker++;
        } else {
            zeroTracker--;
        }
    }
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
    return numAgents - numFickle;
}

/**
 * @brief : return size of initial Ones
 * 
 * @return int 
 */
int Population::oneSize() const {
    return numOnes;
}

/**
 * @brief : see whether consensus has been reached
 * 
 * @return true : if so
 * @return false : if not
 */
bool Population::consensusReached() {
    if (zeroTracker == 0 || zeroTracker == numAgents) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief : return the consensus value
 * 
 * @return int 
 */
int Population::consensusValue() {
    if (zeroTracker == 0) {
        return 1;
    } else if (zeroTracker == numAgents) {
        return 0;
    } else {
        return -1;
    }
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
    out << "Population Size: " << numAgents << endl;
    for(int i = 0; i < numAgents; i++) {
        out << agents[i];
    }
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