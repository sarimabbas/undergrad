/*
 * main.cpp
 *
 *  Created on: Nov 15, 2010
 *  Author: Michael Fischer
 *      for use in Yale course CPSC 427a, Fall 2010
 *
 */

#include "stack.hpp"
#include <iostream>
using namespace std;

//------------------------------------------------------------------------------
int main() {
    try {
        // Create two different types of stack
        Stack<int> y;
        Stack<double> dbl;

        // Push some items onto the stacks
        y.push(2);
        y.push(3);
        dbl.push(6.3);

        // throw "test string";  // uncomment to exercise catc(...)

        // Pop the stack, forcing an exception
        for (;;) {
            cout << "Attempting to pop from stack y: ";
            cout << "popped " << y.pop() << endl;
        }
    } catch (empty_stack_error& obj) {
        // report error
        cout << "Exception caught: " << obj.what() << endl;
        cout << "Error exit" << endl;
        return 1;
    } catch (...) {
        cout << "Unhandled exception caught by main()" << endl;
    }
}
