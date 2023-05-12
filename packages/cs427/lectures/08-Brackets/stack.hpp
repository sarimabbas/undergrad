// ============================================================================
// Name: Bracket-matching example of stack usage                File: stack.hpp
// Author:  Michael and Alice Fischer                   Copyright: January 2009
// Modified September 2016
// ============================================================================
#ifndef STACK_HPP
#define STACK_HPP

#include "tools.hpp"
#include "token.hpp"

#define INIT_DEPTH 1    // initial stack size; stack grows as needed
typedef Token T;        // T is copyable type of stack items

//--------------------------------- Type definition for stack of type T items
class Stack {
private:
    int max = INIT_DEPTH;   // Number of slots in stack.
    int top = 0;            // Stack cursor.
    T* s = new T[max];      // Pointer to stack base.
    string name;            // Print name of this stack.

public:
    //-------------------------------------------------------- Constructors
    Stack(const string& name) : name(name) {}
    ~Stack() {
        delete[] s;
        cout << "Running destructor for stack " << name << endl;
    }
    Stack( const Stack& )            =delete;   // No copy constructor
    Stack& operator=( const Stack& ) =delete;   // No copy assignment
    Stack( Stack&& )                 =default;  // Default move constructor
    Stack& operator=( Stack&& )      =default;  // Default move assignment

    //-------------------------------------------------------- Prototypes
    ostream& print(ostream& out) const;
    void push(T c);
    //-------------------------------------------------------- Inline functions
    T pop() {return s[--top];}            // Pop and return top item.
    T peek() {return s[top - 1];}         // Return top item without popping.
    bool empty() const {return top == 0;} // Return true iff stack is empty
    int size() {return top;}              // Number of items on the stack.
};

inline ostream& operator<<( ostream& out, const Stack& stk ) {
    return stk.print( out );
}

#endif
