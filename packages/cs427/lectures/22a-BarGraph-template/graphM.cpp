//==========================================================================
// Bargraph of Exam Scores: An introduction to class interactions.
// Read scores from a user-specified file; display them as a bar graph.
// A. Fischer, September 30, 2000                           file: graphM.cpp
// Modified M. & A. Fischer, September 17, 2009
//==========================================================================
#include "tools.hpp"
#include "graph.hpp"

// Static function prototype
static
void run( void );

//-----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    banner();
    try {
        run();
    } catch (Fatal& e) {
        cerr << "Catching Fatal exception\n" << e.what() << endl;
    } catch (...) {
        std::cerr << "Uncaught exception" << std::endl;
    }
    bye();
}

void run ( void )
{
    Graph::instructions();          // Call of static class method
    char fname [80];
    cout << "Name of data file: ";
    // cin >> fname;                   // *** IRRESPONSIBLE CODE ***
    cin.getline(fname, 80);         // Prevents buffer overrun
    ifstream infile (fname);        // Declare and open input stream
    if (!infile)  throw Fatal( "Cannot open %s for input - aborting!!\n", fname );
    else say ( "File is open and ready to read.\n");

    Graph curve( infile );          // Declare and construct a Graph object.
                                    // Realizes data structure from a file
    cout <<curve;                   // Print the graph.
    // OR: curve.print( cout );     // Print the graph.

    // Storage belonging to curve will be freed by destructors at this time.
}
