/*
 * except.cpp
 *
 *  Created on: Nov 16, 2016
 *      Author: mike
 */

// Exceptions demo

#include <iostream>
using namespace std;

// -----------------------------------------------------------------------------
// Exception class
class AgeError {
  string msg;
public:
  AgeError(string s) : msg(s) {}
  ostream& printError(ostream& out) const { return out<< msg; }
};

// -----------------------------------------------------------------------------
// Sample client class that generates an exception
class Age {
public:
  void run() { 
    int age;
    cout<< "I'm running"<< endl;
    cout<< "What is your age? ";
    cin>> age;
    if (age<0) throw AgeError("Age can't be negative");
    cout << "Your age is " << age << endl;
  }
};

// -----------------------------------------------------------------------------
// Catching an exception
int main() {
  cout<< "Type negative age to cause exception.\n"
      << "Type non-negative age to see normal exit."<< endl; 

  try {
    Age a;
    a.run();
  }
  catch (AgeError& aerr) {
    // report error
    cout<< "Age error: ";
    aerr.printError( cout )<< endl;  
    cout<< "Error exit"<< endl;
    return 1;
  }
  cout<< "Normal exit"<< endl;
}
