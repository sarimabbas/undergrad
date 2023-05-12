// ============================================================================
// Name: Consensus test executable main                 File: test.cpp
// Author:  Sarim Abbas                                 Copyright: October 2018
// Modified October 2018
// ============================================================================
#include "agent.hpp"
#include "simulator.hpp"
#include "tools.hpp"

int main(void) {
    banner();
    cout << "TEST SCRIPT" << endl;

    cout << "Test Agent" << endl;
    Agent a(0);
    cout << a << endl;

    cout << "Test Simulator" << endl;
    Simulator sims(20, 3, 3);
    cout << sims << endl;
    // add more tests below
    
    bye();
}