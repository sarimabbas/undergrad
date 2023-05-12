#include <iostream>
#include <string>
#include <fstream>

using namespace std;    

int main(void) {
    ifstream in("hello.txt");
    
    double x;
    // do {
    //     in >> x;
    //     if(!in.fail()) {
    //         cout << x << endl;
    //     }
    // } while(in.good());

    // do {
    //     in >> x;
    //     if (!in.fail()) {
    //         cout << x << endl;
    //     } 
    // } while (in.good());

    // string one = "hello";
    // string two = "world";
    // one = two;
    // cout << one;

    // double n;
    // for (;;) {
    //     in >> n;

    //     if (in.eof() || in.bad()) {
    //         break;
    //     } else if (in.fail()) {
    //         in.clear();  // unset failbit
    //         in.ignore(1);  // skip bad in
    //     } else {
    //         std::cout << n << endl;
    //     }
    // }

    cout << "\nReading numbers.\n";
    double number;
    for (;;) {
        in >> number;
        if (in.good()) {
            cout << number << endl;
            cout << "BYTE POSITION: " << in.tellg() << endl;
        }
        else if (in.eof()) {
            break;
        }
        else if (in.fail()) {
            cout << "BYTE POSITION: " << in.tellg() << endl;
            in.clear();
            cout << "BYTE POSITION: " << in.tellg() << endl;
            in.ignore(1);
        } else if (in.bad()) {
            cout << "Bad error while reading input stream." << endl;
            break;
        } 
    }



}

