// File: tools.hpp ----------------------------------------------------
// Header file for the C++ tools library.
// Authors:  Alice E. Fischer and Michael J. Fischer.
// Modified September 2012; September 2016.

#pragma once

// -------------------------------------------------------------------
// Local definitions.
// Please enter your own name.
// -------------------------------------------------------------------
#define NAME    "Ima Goetting Closeau"
#define CLASS   "CPSC 427/527"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

#include <cstdio>
#include <cstdlib>     // for malloc() and calloc()
#include <cstring>     // for time_t, time() and ctime()
#include <cerrno>
#include <cmath>
#include <ctime>
#include <cctype>      // for isspace() and isdigit()
#include <cstdarg>     // for functions with variable # of arguments
#include <unistd.h>

using namespace std;

// -------------------------------------------------------------------
// Macros to make more convenient use of standard library functions.
// -------------------------------------------------------------------
#define DUMPp(p) "\n"<<hex<<"   " #p " @ "<<(unsigned)&p<<"   value = "<<(unsigned)p<<"   " #p " --> "<<dec<<*p
#define DUMPv(k) "\n"<<hex<<"   " #k " @ "<<(unsigned)&k<<"   value = "<<dec<<k

// -------------------------------------------------------------------
// Routine screen and process management.-----------------------------
// -------------------------------------------------------------------
void     fbanner( ostream& fout );
#define  banner()  fbanner( cout )
void     bye( void );
void     say( const char* format, ... );
void     delay( int );

// -------------------------------------------------------------------
// I/O Extension. ----------------------------------------------------
// -------------------------------------------------------------------
istream&  flush( istream& is );     // Used in cin >>x >>flush;

// -------------------------------------------------------------------
// Error handling. ---------------------------------------------------
// -------------------------------------------------------------------
// This class is for fatal error exceptions.
//    The constructor takes a format argument followed by any number
//    of data arguments.
//    It formats and stores an error message of at most 255 bytes.
//    The what() member function returns the stored string.
class Fatal {
private:
        char msg[256];
public:
        Fatal( const char* format, ... );
        const char* what() const { return msg; }
};

// -------------------------------------------------------------------
// Time and date. ----------------------------------------------------
// -------------------------------------------------------------------
void   when( char* date, char* hour );
char*  today( char* date );
char*  oclock( char* hour );

