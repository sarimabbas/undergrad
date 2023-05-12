
// ==========================================================================
// Template class definition for a stack of T objects
// Alice E. Fischer  June 10, 2000                          file: stackT.hpp
//
#pragma once

#include "flexT.hpp"

template <class T>
class Stack : FlexArray<T> {
    const char* Name;
  public:           //-------------------------------------------------------
    Stack(const char* nm, int sz=4): FlexArray<T>(sz), Name(nm) {
        cout << " Create " <<Name <<"  ";
    }
    Stack(const Stack<T>& s){ throw Fatal( " Can't shallow-copy stack %s", s.Name ); }
    ~Stack() { cout << " Delete " <<Name <<"  ";}

    void  push(T& c)    { this->put( c ); };
    T     pop()         { return empty()
                            ? (T)0 : FlexArray<T>::store[--FlexArray<T>::n]; }
    T     top()   const { return empty()
                            ? (T)0 : FlexArray<T>::store[FlexArray<T>::n-1]; }
    bool  empty() const { return FlexArray<T>::n == 0; }
    int   depth() const { return FlexArray<T>::n; }
    ostream& print(ostream& out) const;
};

template <class T> //--------------------------------------------------------
ostream&
Stack<T>::print(ostream& out) const {
    out <<"\nStack has " <<depth() <<" items: bottom<[";
    FlexArray<T>::print( out );
    out << " ]>top\n";
    return out;
}

template <class T> inline //-------------------------------------------------
ostream& operator<< ( ostream& out, Stack<T>& s ){ return s.print( out ); }
