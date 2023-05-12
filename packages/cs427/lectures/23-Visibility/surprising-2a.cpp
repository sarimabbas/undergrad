// Surprising example 2a
// Try to compile this file using
// g++ -Wall -std=c++17 surprising-2a.cpp

class A { };
class B : public A {};    // <-- public derivation

int main() {
    A* ap;
    B* bp;
    ap = bp;
}
