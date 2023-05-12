// -----------------------------------------------------------------------------
// Data class                                                           item.hpp
// A. Fischer and M. Fischer, September 2010
//      Revised September 2018
// -----------------------------------------------------------------------------
#ifndef ITEM_HPP
#define ITEM_HPP

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>
using namespace std;

//----------------------------------------------------------------------
class Item {
  private:
    double price=0;	    // Price for one item.
    string name; 	    // Name of the product.

  public:  //-------------------------------------------------------------------
    // Normal constructors and destructor.
    Item() =default;
    Item( double p, string d ) : price(p), name(d) {}
    ~Item() =default;

    //--------------------------------------------------------------------------
    // Prohibit use of copy constructor and copy assignment
    Item( const Item& ) = delete;
    Item& operator=( const Item& ) = delete;
    //--------------------------------------------------------------------------
    // Move constructor and move assignment
    Item( Item&& data ) : price( move(data.price) ),
    					  name( move(data.name) ) {};

    Item& operator=( Item&& data ) {
        price = data.price;
        name = move(data.name);
        return *this;
    }
    //--------------------------------------------------------------------------
    bool match( const string& key ) const { return key==name; }
    bool operator<( const Item& rhs ) const { return name < rhs.name; }

    ostream& print( ostream& out ){
        cout<< right << setw(7)<< setprecision(2) << fixed << price
            << "  " << left << name;
        return out;
    }
};
inline ostream& operator <<(ostream& out, Item& it){ return it.print(out); }
#endif

// Local Variables:
// tab-width: 4
// End:
