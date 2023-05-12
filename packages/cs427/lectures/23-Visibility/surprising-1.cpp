// Surprising example 1
// Try to compile this file using
// g++ -Wall -std=c++17 surprising-1.cpp

class A { 
protected:
  int x;
};

class B : public A {
public:
  int f() { return x; }         //  ok
  int g(A* a) { return a->x; }  //  privacy violation
};

int main() {
}
