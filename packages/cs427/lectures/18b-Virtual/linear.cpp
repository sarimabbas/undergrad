// ------------------------------------------------------------------------
// Linear Containers              
// A. Fischer June 12, 2001                                file: linear.cpp
// ------------------------------------------------------------------------
#include "linear.hpp"   
// ------------------------------------------------------------------------
Linear::~Linear () { 
    for (reset(); !end(); ++*this) {
        delete prior->data;
        delete prior;
    }
    delete prior->data;
    delete prior;
}
// ------------------------------ Move index to next item in the container.
void 
Linear::operator ++() { 
    if (!end() ){
        prior = here; 
        here = here->next;
    }
}
// ---------------- Put an item into the container between prior and here.
// ------------- Assumes that here and prior have been positioned already.
void 
Linear::insert(Cell* cp) {
    cp->next = here; 
    here = prior->next = cp; 
}
// ---------------- Take an item out of the container. Like pop or dequeue.
// -- Assumes that here and prior have been positioned to the desired item.
Cell* 
Linear::remove() {  
    if (! here ) return nullptr;
    Cell* temp = here;                  // Grab cell to remove.
    here = prior->next = temp->next;    // Link around it. 
    return temp;
}
// ------------------------------------ Remove a Cell and return its Item.
Item* 
Linear::pop(){
    focus();                 // Set here and prior for deletion point.
    Cell* temp = remove();   // Remove first real cell on list. 
    if (!temp) return nullptr;  // Check for empty condition.
    Item* answer = *temp;    // Using cast coercion from Cell to Item.
    delete temp;             // Take contents out of cell, delete Cell. 
    return answer;           // Return data item.       
}
// ---------------------------------------- Print the container's contents.
ostream& 
Linear::print (ostream& out ) {
    out << "  <[\n";
    for (reset(); !end(); ++*this)  out << "\t" <<*here;
    return out << "  ]>\n";
};
