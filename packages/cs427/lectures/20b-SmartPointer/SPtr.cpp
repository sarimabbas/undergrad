/*
 * SPtr.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: mike
 */

#include "SPtr.hpp"

// Default constructor
SPtr::SPtr() : count(nullptr), target(nullptr) {
    cout << "  Default constructor called for Sptr " << id() << endl;
}

// Construct an SPtr to manage p
SPtr::SPtr(T* p) : count(new unsigned(1)), target(p) {
    cout << "  SPtr(T*) constructor called for Sptr " << id() << endl;
}

//------------------------------------------------------------------------------
// Copy constructor
SPtr::SPtr(const SPtr& sp) : count(sp.count), target(sp.target) {
    ++*count;
    cout << "  Copy constructor from Sptr " << sp.id() << " to " << id() << endl;
}

//------------------------------------------------------------------------------
// Copy assignment
SPtr&
SPtr::operator=(const SPtr& sp) {
    cout << ">>> calling reset" << endl;
    reset();
    cout << ">>> reset returned" << endl;
    count = sp.count;
    target = sp.target; // copy pointer
    ++*count;
    cout << "  Copy assignment from Sptr " << sp.id() << " to " << id() << endl;
    return *this;
}

//------------------------------------------------------------------------------
// Move constructor
SPtr::SPtr(SPtr&& sp) : count(sp.count), target(sp.target) {
    sp.count = nullptr;
    sp.target = nullptr;
    cout << "  Move constructor from Sptr " << sp.id() << " to " << id() << endl;
}

//------------------------------------------------------------------------------
// Move assignment
SPtr& SPtr::
operator=(SPtr&& sp) {
    reset();
    count = sp.count;
    target = sp.target;
    sp.count = nullptr;
    sp.target = nullptr;
    cout << "  Move assignment from Sptr " << sp.id() << " to " << id() << endl;
    return *this;
}

//------------------------------------------------------------------------------
// Make *this no longer point to anything, and delete the count and target
// if this was the last pointer to that object.
void SPtr::reset() {
    if (empty()) return;        // already empty; nothing to do
    // unmanage target pointer
    if (--*count > 0) return;
    // no other references, so release managed storage
    cout << "  Releasing storage managed by Sptr " << id() << endl;
    delete count;
    delete target;
    count = nullptr;
    target = nullptr;
}

