//===========================================================================
// Declarations for a bar graph.
// A. Fischer, April 22, 2000                                 file: graph.hpp
// Modified M. & A. Fischer, September 17, 2009
//===========================================================================
#pragma once
#include "tools.hpp"
//#include "row.hpp"
#include "rowNest.hpp"  // alternative way to define a recursive type
#define BARS 11

//---------------------------------------------------------------------------
class Graph {
  private:
    Row<Item>* bar[BARS];   // Each list is one bar of the graph.  (Aggregation)
    void insert( char* name, int score );

  public:
    Graph ( istream& infile );
    ~Graph();
    ostream& print ( ostream& out ) const;
    // Static functions are called without a class instance
    static void instructions() {
        cout <<"Put input files in same directory as the executable code.\n";
    }
};
inline ostream& operator<<( ostream& out, const Graph& G){ return G.print( out ); }