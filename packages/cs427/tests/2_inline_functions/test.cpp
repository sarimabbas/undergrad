#include <iostream>
#include <string>

#include "flipflop.hpp"

int main(void) {
    FlipFlop test;
    test.setState( 'L' );
    test.flip();
    cout << "FLIPFLOP PRINT: " << test << endl;
}