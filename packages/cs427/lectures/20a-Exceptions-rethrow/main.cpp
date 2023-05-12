/*
 * main.cpp
 *
 *  Created on: Nov 22, 2012
 *      Author: mike
 */

/* Illustrates the use of rethrow and the processing of a
 * single exception by multiple catch blocks.
 */

#include <iostream>
using namespace std;

//-----------------------------------------------------------------------------
class MyException {
public:
    MyException() {
    }
    MyException(MyException& e) {
        cout << "Copy constructor called\n";
    }
    ~MyException() {
    }
} myex;  // declares class and instantiates it

//-----------------------------------------------------------------------------
int main() {
    try {
        try {
            throw myex;
        } catch (MyException& e) {
            cout << "Exception caught by inner catch\n";
            throw;
        }
    } catch (MyException& err) {
        cout << "Exception caught by outer catch\n";
    }
    return 0;
}
