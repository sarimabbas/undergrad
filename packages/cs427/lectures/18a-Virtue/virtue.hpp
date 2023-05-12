/*
 *  virtue.hpp
 *  Virtue
 *  Created by Alice Fischer on 3/31/09.
 */
#include <iostream>
using namespace std;

class Basic {
public:
    virtual ~Basic() {}
    virtual void print(){cout <<"I am basic.  "; }
};

class Virtue : public Basic {
public:
    virtual void print(){cout <<"I have virtue.  "; }
};

class Question : public Virtue {
public:
    void print(){cout <<"I am questing.  "; }
};
