// ------------------------------------------------------------------------
// A cell contains an Item* and a link.  Cells are used to build lists.
// A. Fischer   June 13, 2000                               file: cell.hpp
// ------------------------------------------------------------------------
#pragma once

#include <iostream>
#include "item.hpp"
using namespace std;

// ------------------------------------------------------------------------

class Cell {
  friend class Linear;
  friend ostream& operator<<( ostream& out, Cell& c);

  private:  // ------------------------------------------------------------
    Item* data;
    Cell* next;

    Cell(Item* e = nullptr, Cell* p = nullptr ): data(e), next(p){ }
    ~Cell(){ cout <<"\n  Deleting Cell " <<this << dec <<"..."; }
    operator Item*() { return data; }   // Cast Cell to Item*. ------------

    void print(ostream& out) const {    // --------------------------------
        if (data) {
            out << "Cell " << this;
            out << " [" << *data << ", " << next << "]\n";
        }
    }
};
inline ostream& operator<<(ostream& out, Cell& c){c.print(out); return out;}

