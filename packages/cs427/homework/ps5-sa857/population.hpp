#pragma once
#include "fickle.hpp"
#include "crowd.hpp"

class Population {
    private:
        // agents
        Agent** agents;

        // counts
        int numAgents;
        int numOnes;
        int numFickle;
        
        // tracker
        int zeroTracker;

        // random real gen
        double dRandom();

    public:
        // constructors
        Population() = delete;
        Population( int numAgents, double probFickle, double probOne );

        // pop sizes
        int size() const;
        int fickleSize() const;
        int crowdSize() const;
        int oneSize() const;

        // send message
        void sendMessage( int sender, int receiver );

        // indicators
        bool consensusReached();
        int consensusValue();

        // print function
        ostream& print(ostream& out) const;

        // destructor
        ~Population();
};

inline ostream& operator<<(ostream& out, const Population& p) {
    return p.print(out);
}
