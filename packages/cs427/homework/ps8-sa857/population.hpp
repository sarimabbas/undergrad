// ============================================================================
// Name: Population class declarations                  File: population.hpp
// Author:  Sarim Abbas                                 Copyright: November 2018
// Modified November 2018
// ============================================================================
#pragma once
#include "multiagent.hpp"

class Population {
    private:
        // agents
        Agent** agents;

        // counts
        int numAgents;
        int numNak;
        int numFickle;
        int numCrowd;

        // probs
        double probNak;
        double probFickle;
        double probExtend;

        // random real gen
        double dRandom();

    public:
        // constructors
        Population() = delete;
        Population( int numAgents, double probNak, double probFickle, double probExtend );

        // pop sizes
        int size() const;
        int fickleSize() const;
        int crowdSize() const;
        int nakSize() const;

        // send message
        void sendMessage( int sender, int receiver );

        // extend blockchain
        void extend( int extender );

        // get choice register
        Blockchain agentChoice( int index ) const;

        // print functions
        ostream& print(ostream& out) const;
        ostream& printStats(ostream& out) const;

        // destructor
        ~Population();
};

/**
 * @brief : extend << operator to print a population
 *
 * @param out : stream to modify
 * @param p : population to print
 * @return ostream& : modified stream
 */
inline ostream& operator<<(ostream& out, const Population& p) {
    return p.print(out);
}
