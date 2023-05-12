//                  -*- mode:c++; tab-width:4 -*-
// File: main.cpp ----------------------------------------------------------
// Implementation files for readint app.
// Author: Sarim Abbas.
// Created September 2018. Modified September 2018.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "tools.hpp"
#include <string.h>

void run( const string& input_file );

// ---------------------------------------------------------------------------
//  Calls functions to show a bannner, collect user info, and exit.

int main(int argc, char* argv[]) {
    banner();

    if (argc > 1) {
        run( argv[1] );
    } else {
        fatal( "Usage: readint input_file" );
    }
    
    bye();
}

// ---------------------------------------------------------------------------
//  Collect input from user and display this input and user's age.

void run( const string& input_file ) {
    // open file
    ifstream in( input_file );
    if (!in) fatal( "Cannot open input_file" );

    int x;
    char ch;
    int accumulator = 0;
    do {
        // read input into integer
        in >> x;

        // check for failure first (for readability)
        if (!in.fail()) {
            accumulator += x; 
            cout << accumulator << endl;
            accumulator = 0;
        } else {
            // reset stream
            in.clear();
            in.get( ch );

            // convert to unsigned char (as per email)
            unsigned char uch = ch;

            // output character in ASCII dec
            cout << "Skipping char: " << right << setw( 3 ) << setfill( ' ' ) << (int) uch;
            // output character in hex
            cout << " 0x" << right << setw( 2 ) << setfill( '0' ) << hex << (int) uch;
            // output printable character
            if (isprint( uch )) {
                cout << " " << dec << "\'" << uch << "\'";
            }

            // end line and flush
            cout << dec << endl;
        }
    } while(in.good());

    // check if bad data read
    if (!in.eof()) {
        fatal( "I/O error or bad data" );
    }

    // close stream
    in.close();

    // exit
    cout << "Loop exit";
}
