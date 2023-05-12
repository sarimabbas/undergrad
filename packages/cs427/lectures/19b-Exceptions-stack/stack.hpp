/*
 * stack.hpp
 *
 *  Created on: Nov 15, 2010; modified April 14, 2016
 *  Author: Michael Fischer
 *      for use in Yale course CPSC 427a, Fall 2010 and Spring 2016
 *
 *  Derived from "Exception Handling: A False Sense of Security", by
 *      Tom Cargill, C++ Report, Volume 6, Number 9, November-December
 *      1994, available at
 *      http://ptgmedia.pearsoncmg.com/images/020163371x/supplements/Exception_Handling_Article.html
 *
 *      Tom's code in turn was derived from David Reed, Exceptions:
 *      Pragmatic issues with a new language feature", C++ Report,
 *      October 1993, page 42.
 *
 */

#pragma once

// Stack template class that throws exceptions on errors
// bad_alloc thrown for out of memory
// empty_stack_error thrown for attempting to pop an empty stack

#include <string>

//-----------------------------------------------------------------------------
// Exception class
class empty_stack_error {
    const std::string msg;
public:
    empty_stack_error(std::string s) :
        msg(s) {
    }
    const std::string& what() const {
        return msg;
    }
};

//-----------------------------------------------------------------------------
template<class T>
class Stack {
private:
    unsigned nElems; // allocation size
    unsigned size; // number of elements in stack
    T* stk; // array of stack elements
    void grow(); // increases stack allocation
public:
    unsigned getSize() const;
    void push(T);
    T pop();

    Stack();
    ~Stack();
    Stack(const Stack&);
    Stack& operator=(const Stack&);
};

//-----------------------------------------------------------------------------
// Default constructor
template<class T>
Stack<T>::Stack() {
    size = 0;
    stk = new T[nElems = 10];
}

//-----------------------------------------------------------------------------
// Copy constructor
template<class T>
Stack<T>::Stack(const Stack<T>& s) {
    size = s.size;
    stk = new T[nElems = s.nElems];
    for (unsigned k = 0; k < size; k++)
        stk[k] = s.stk[k];
}

//-----------------------------------------------------------------------------
// Destructor
template<class T>
Stack<T>::~Stack() {
    delete[] stk;
}

//-----------------------------------------------------------------------------
// Grow
template<class T>
void Stack<T>::grow() {
    T* newBuffer = new T[nElems *= 2];
    for (unsigned k = 0; k < size; k++)
        newBuffer[k] = stk[k];
    delete[] stk;
    stk = newBuffer;
}

//-----------------------------------------------------------------------------
// Push
template<class T>
void Stack<T>::push(T element) {
    if (size == nElems)
        grow();
    stk[size++] = element;
}

//-----------------------------------------------------------------------------
// Pop stack
template<class T>
T Stack<T>::pop() {
    if (size == 0)
        throw empty_stack_error("pop on empty stack");
    return stk[--size];
}

//-----------------------------------------------------------------------------
// Size of stack
template<class T>
unsigned Stack<T>::getSize() const {
    return size;
}

//-----------------------------------------------------------------------------
// Assignment
template<class T>
Stack<T>&
Stack<T>::operator=(const Stack& s) {
    delete[] stk;
    size = s.size;
    stk = new T[nElems = s.nElems];
    for (unsigned k = 0; k < size; k++)
        stk[k] = s.stk[k];
    return *this;
}
