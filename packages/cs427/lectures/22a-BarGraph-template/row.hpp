//===========================================================================
// Class for a linked-list row and its cells
// A. Fischer, October 1, 2000                                  file: row.hpp
// Modified M. & A. Fischer, September 17, 2009
//===========================================================================
#pragma once
#include <iostream>
#include "item.hpp"
using namespace std;

//---------------------------------------------------------------------------
// Dependent class.  Holds an Item and a link to another Cell
template <class T>
class Cell {
    friend class Row<T>;
private:
    T* data;              // Pointer to one data item    (Aggregation)
    Cell* next;           // Pointer to next cell in row (Association)

    Cell(char* d, int s, Cell* nx) {
        data = new T(d, s);
        next = nx;
    }
    ~Cell() {
        delete data;
        cerr << "  Deleting Cell " << "\n";
    }
};

//---------------------------------------------------------------------------
// Data structure class
template <class T>
class Row { // Interface class for one bar of the bargraph.
private:
    char label[10];             // Row header label
    Cell<T>* head;              // Pointer to head of row

public:
    Row(int n);
    ~Row();
    void insert(char* name, int score);  // delegation
    ostream& print(ostream& os) const;
};

//---------------------------------------------------------------------------
template <class T>
inline ostream& operator <<(ostream& out, const Row<T>& r) {
    return r.print(out);
}

//---------------------------------------------------------------------------
// Row number is used to construct a label for the row
template <class T>
Row<T>::Row( int rowNum ){
    if (rowNum == 10) strcpy( label, "Errors:  " );
    else {
        strcpy( label, " 0.. 9:  " );
        label[0] = label[4] = '0'+ rowNum;  // example: label=="70..79"
    }
    head = NULL;  // represents empty list of data
}
//---------------------------------------------------------------------------
// Row is responsible for deleting everything created by this class
template <class T>
Row<T>::~Row(){
    Cell<T>* curr;
    while (head != NULL){
        curr=head;
        head=head->next;
        delete curr;
    }
    cerr << " Deleting row " << label <<" >>> \n";
}
//---------------------------------------------------------------------------
// Create and insert Cell into linked list at head
// Design pattern: creator.  Item is created by Cell constructor.
template <class T>
void
Row<T>::insert( char* name, int score ){
    head = new Cell<T>( name, score, head );  // put new cell at head of list
}
//---------------------------------------------------------------------------
// Design decision: print Cell data directly; no delegation of print
template <class T>
ostream&
Row<T>::print( ostream& os ) const {
    Cell<T>* curr;
    os << label;
    for (curr=head; curr!=NULL; curr=curr->next)
        curr->data->print( os );    //  OR: os << *(curr->data);
    return os;
}

