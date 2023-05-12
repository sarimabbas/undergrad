// Surprising example 2b
// Try to compile this file using
// g++ -Wall -std=c++17 surprising-2b.cpp

class A { };
class B : private A {};    // <-- private derivation

int main() {
    A* ap;
    B* bp;
    ap = bp;
}
