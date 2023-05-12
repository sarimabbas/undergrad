// ============================================================================
// Name: Bracket-matching example of stack usage                File: stack.cpp
// Author:  Michael and Alice Fischer                   Copyright: January 2009
// Modified September 2016
// ============================================================================
#include "stack.hpp"

//-----------------------------------------------------------------------
// Prints the stack between markers "Bottom~~" and "~~Top".

std::ostream& Stack::
print(ostream& out) const {
    out << "The " << name << " stack contains: Bottom~~";
    T* p = s;                                // Scanner & end pointer for data
    T* pend = s + top;
    for (; p < pend; ++p) out << ' ' << *p;
    return out << " ~~Top";
}

//-----------------------------------------------------------------------------
// Push c onto the stack.  Grow the stack if full.
void Stack::
push(T c) {
    if (top == max) {       // If stack is full, allocate more space.
        cout << "-Doubling stack length-";
        T* temp = s;                               // grab old array.
        s = new T[max *= 2];                       // make bigger one,
        for (int k=0; k<top; ++k) s[k] = temp[k];  // copy over old data
        delete[] temp;                             // free old array.
    }
    s[top++] = c;           // Store data in array, prepare for next push.
}
