//==========================================================================
// Implementation of the Graph class.
// A. Fischer, April 22, 2000                                file: graph.cpp
// Modified M. & A. Fischer, September 17, 2009
//==========================================================================
#include "graph.hpp"
//--------------------------------------- Use an input file to build a graph.
Graph::Graph( istream& infile ){
    char initials [4];
    int score;
    for (int k=0; k<BARS; ++k) bar[k] = new Row<Item>(k);
    for (;;){
        infile >> ws;               // Skip leading whitespace before get.
        infile.get(initials, 4, ' ');  // Read three initials ... safely.
        if (infile.eof()) break;
        infile >> score;            // No need to skip ws before using >>.
        insert (initials, score);   // *** POTENTIAL INFINITE LOOP ***
    }
}

// --------------------------------------------------------- Delete all Rows.
Graph::~Graph() {
    for (int k=0; k<BARS; ++k)  delete bar[k];
    cerr <<" Deleting Graph\n";
}

// ------------------------------------------------ Insert a node into a Row.
void
Graph::insert( char *initials, int score ){ // Function is private within class.
    const int intervals = BARS-1;
    int index;
    // Calculate insertion index for score
    if (score >= 0 && score < 100)      // If score is between 0-99, it
        index = (score / intervals);    // belongs in one of first BARS-1 rows.
    else
        index = BARS-1;                 // Errors are displayed on last row
    bar[index]->insert( initials, score );  // delegation
}

// ---------------------------------------------- Print the entire bar graph.
ostream&
Graph::print( ostream& out ) const {
    out << "\n";
    for (int k=0; k<BARS; ++k)
        out << *bar[k] <<"\n";          // Delegate to Row::print()
    return out;
}
