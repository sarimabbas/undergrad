//===========================================================================
// Implementation of class Row.
// A. Fischer, April 22, 2000                                   file: row.cpp
// Modified M. & A. Fischer, September 17, 2009
//===========================================================================
#include "row.hpp"
//---------------------------------------------------------------------------
// Row number is used to construct a label for the row
Row::Row( int rowNum ){
    if (rowNum == 10) strcpy( label, "Errors:  " );
    else {
        strcpy( label, " 0.. 9:  " );
        label[0] = label[4] = '0'+ rowNum;  // example: label=="70..79"
    }
    head = nullptr;  // represents empty list of data
}
//---------------------------------------------------------------------------
// Row is responsible for deleting everything created by this class
Row::~Row(){
    Cell* curr;
    while (head != nullptr){
        curr=head;
        head=head->next;
        delete curr;
    }
    cerr << " Deleting row " << label <<" >>> \n";
}
//---------------------------------------------------------------------------
// Create and insert Cell into linked list at head
// Design pattern: creator.  Item is created by Cell constructor.
void
Row::insert( char* name, int score ){
    head = new Cell( name, score, head );  // put new cell at head of list
}
//---------------------------------------------------------------------------
// Design decision: print Cell data directly; no delegation of print
ostream&
Row::print( ostream& os ) const {
    Cell* curr;
    os << label;
    for (curr=head; curr!=nullptr; curr=curr->next)
        curr->data->print( os );    //  OR: os << *(curr->data);
    return os;
}
