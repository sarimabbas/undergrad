//==============================================================================
// Project: Bracket-matching example of stack usage               File: main.cpp
// Author:  Michael and Alice Fischer                    Copyright: January 2009
// Modified September 2016
// =============================================================================
#include "tools.hpp"    
#include "brackets.hpp"

// Static function prototype
static
void run(int argc, char* argv[]);

//-----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    banner();
    run(argc, argv);
    bye();
}

void run(int argc, char* argv[]) {
    cout << "Welcome to the bracket checker!";

    if (argc != 2)
        fatal("usage: %s file", argv[0]);
    cout << "Checking file '" << argv[1] << "'";

    ifstream in(argv[1]);
    if (!in)
        fatal("can't open file '%s' for reading", argv[1]);

    Brackets b;         // Declare and initialize the application class.
    b.checkFile(in);    // Execute the primary application function.
    in.close();
}
