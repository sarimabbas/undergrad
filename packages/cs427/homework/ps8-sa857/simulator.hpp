// ============================================================================
// Name: Consensus Simulator class declarations         File: simulator.hpp
// Author:  Sarim Abbas                                 Copyright: November 2018
// Modified November 2018
// ============================================================================
#pragma once
#include "population.hpp"
#include "inventory.hpp"

class Simulator {
    private:
        // inventory of Blockchains
        Inventory inventory;

        // dynamic aggregation of agents
        Population& pop;

        // random number generator
        int uRandom( int n );

        // rounds elapsed
        int maxRounds;

        // probability of calling extend()
        double probExtend;

        // random real gen
        double dRandom();

    public:
        // default constructor
        Simulator() = delete;

        // custom constructor
        Simulator( Population& pop, int maxRounds, double probExtend );

        // simulate
        void run();

        // max rounds
        int rounds() const;

        // print inventory
        ostream& printInventory(ostream& out);
};
