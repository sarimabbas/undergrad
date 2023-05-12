// ------------------------------------------------------------------------
// Abstract Containers
// A. Fischer   June 10, 2001                             file: contain.hpp
// ------------------------------------------------------------------------
#pragma once

#include "item.hpp"

class Container {
  public:       // -----------------------------------------------------
    virtual ~Container() {}
    virtual void     put(Item*)      =0; // Put Item into the Container.
    virtual Item*    pop()           =0; // Remove next Item from Container.
    virtual Item*    peek()          =0; // Look but don't remove next Item.
    virtual ostream& print(ostream&) =0; // Print all Items in Container.
};

