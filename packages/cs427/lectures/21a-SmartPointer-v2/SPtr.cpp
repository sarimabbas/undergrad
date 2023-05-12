/*
 * SPtr.cpp
 *
 *  Created on: Nov. 11, 2018
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
    reset();
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
// Unmanage target if currently being managed and adjust count.
// If count falls to 0, delete count and target.
// Afterwards, *this is back in its default empty state.
void SPtr::reset() {
    if (empty()) return;        // already empty; nothing to do
    // Unmanage target pointer
    if (--*count > 0) return;
    // No other references, so release managed storage
    cout << "  Releasing storage managed by Sptr " << id() << endl;
    delete count;
    delete target;
    count = nullptr;
    target = nullptr;
}

