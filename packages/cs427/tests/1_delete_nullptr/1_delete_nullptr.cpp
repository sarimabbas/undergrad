#include <iostream>
#include <string>

class Example {
    public:
        // public data member
        int* arr;

        // constructor
        Example() {
            // add dynamic extension 
            arr = new int[5];
            for(int i = 0; i < 5; i++) {
                arr[i] = i * 2;
            }
        }

        // destructor
        ~Example() {
            // free dynamic extension
            delete[] arr;
            arr = nullptr;
        }
};

int main(void) {
    // test instantiate an Example object
    Example obj;

    // double delete nullptr does nothing
    int* test = nullptr;
    delete test;
    delete test;

    // delete dangling pointer is more dangerous
    // (gives an error)
    int* test2 = new int;
    *test2 = 100;
    delete test2;
    delete test2;
}
