#include <iostream>
#include "virtue.hpp"
using namespace std;

int main (void) {
    cout << "Searching for Virtue\n";
    Basic* array[3];
    array[0] = new Basic();
    array[1] = new Virtue();
    array[2] = new Question();

    array[0]->print();
    array[1]->print();
    array[2]->print();

    cout << endl;
   return 0;
}
