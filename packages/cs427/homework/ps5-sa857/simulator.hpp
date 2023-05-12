// ============================================================================
// Name: Consensus Simulator class declarations         File: simulator.hpp
// Author:  Sarim Abbas                                 Copyright: November 2018
// Modified November 2018
// ============================================================================
#pragma once
#include "population.hpp"

class Simulator {
    private:
        // dynamic aggregation of agents
        Population& pop;

        // random number generator
        int uRandom( int n );

        // rounds elapsed
        int rounds;

    public:
        // default constructor
        Simulator() = delete;

        // custom constructor
        Simulator( Population& pop );

        // simulate
        void run();

        // rounds elapsed
        int numRounds();

        // consensus value reached
        int consensusValue();
};
