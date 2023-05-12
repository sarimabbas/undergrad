//===========================================================================
// Class for a linked-list row and its cells
// A. Fischer, M. Fischer, Sep 17, 2009                     file: rowNest.hpp
// Modified M. & A. Fischer, September 17, 2009
//===========================================================================
#pragma once
#include <iostream>
#include "item.hpp"
using namespace std;
//---------------------------------------------------------------------------
// Data structure class
class Row { // Interface class for one bar of the bargraph.
//---------------------------------------------------------------------------
private:
    class Cell // Dependent class.  Holds an Item and points to another Cell.
    {
        //  friend class Row;
    public:
        Item* data; // Pointer to one data Item    (Aggregation)
        Cell* next; // Pointer to next cell in row (Association)

        Cell(char* d, int s, Cell* nx) {
            data = new Item(d, s);
            next = nx;
        }
        ~Cell() {
            delete data;
            cerr <<"  Deleting Cell " <<"\n";
        }
    };

//--Members of the master class begin here-----------------------------------
private:
    char label[10]; // Row header label
    Cell* head; // Pointer to head of row

public:
    Row(int n);
    ~Row();
    void insert(char* name, int score);  // delegation
    ostream& print(ostream& os) const;
};

//---------------------------------------------------------------------------
inline ostream& operator <<(ostream& out, const Row& T) {
    return T.print(out);
}

