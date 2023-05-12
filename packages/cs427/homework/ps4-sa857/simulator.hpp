// ============================================================================
// Name: Consensus Simulator class declarations         File: simulator.hpp
// Author:  Sarim Abbas                                 Copyright: October 2018
// Modified October 2018
// ============================================================================
#pragma once

#include <vector>
#include "agent.hpp"
#include "tools.hpp"

using namespace std;

class Simulator {
    private:
        // count of agents with 1 as choice
        int agentOnes;

        // count of agents with 0 as choice
        int agentZeros;

        // vector of all agents
        vector<Agent> agents;

        // communictae between pair of agents
        void communicate( Agent& j, Agent& k );

    public:
        // default constructor
        Simulator() = delete;

        // custom constructor
        Simulator( int numAgents, int numOne, unsigned int seed );

        // simulate
        int run( int& rounds );

        // print
        ostream& print( ostream& out ) const;
};

// helper function
int uRandom( int n );

/**
 * @brief : extension of << to print Simulator objects to stream
 *
 * @param out : stream to print to
 * @param s : Simulator object
 * @return ostream& : modified stream
 */
inline ostream& operator<<( ostream& out, const Simulator& s ) {
    return s.print( out );
}