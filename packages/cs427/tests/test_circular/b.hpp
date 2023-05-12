#pragma once
// #include "a.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>


using namespace std;

class A;
using T = A;

class B {
    private:
        T* obj;
    public:
        void hello();
};