// ============================================================================
// Name: Nakamoto, Fickle and Crowd class declarations                File: multiagent.hpp
// Author:  Sarim Abbas                                 Copyright: November 2018
// Modified November 2018
// ============================================================================
#pragma once
#include "agentbase.hpp"

class Nakamoto : public AgentBase {
   public:
    // constructor
    Nakamoto() = delete;

    // custom constructor
    Nakamoto( Blockchain ch );

    // performs the update to the agent via Nakamoto algorithm
    void update( Blockchain m );
};

class Crowd: public AgentBase {
    private:
        // last choice
        Blockchain last_message;
    public:
        // constructor
        Crowd() = delete;  

        // custom constructor
        Crowd( Blockchain ch );

        // performs the update to the agent via crowd algorithm
        void update( Blockchain m );
}; 

class Fickle: public AgentBase {
    public:
        // constructor
        Fickle() = delete;

        // custom constructor
        Fickle( Blockchain ch );

        // performs the update to the agent via fickle algorithm
        void update( Blockchain m );
};
