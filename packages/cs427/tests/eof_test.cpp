#include <iostream>
#include <string>
#include <fstream>

using namespace std;    

int main(int argc, char** argv) {
    ifstream in( argv[1] );
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