/* file: tools.h ------------------------------------------------------------- */
/* header file for the C tools library.                                        */
/* modified September 2009                                                     */

#ifndef TOOLS_H
#define TOOLS_H

/* --------------------------------------------------------------------------- */
/* Local definitions.                                                          */
/* Please enter your own name.                                                 */
/* --------------------------------------------------------------------------- */
#define NAME    "Ima Goetting Closeur"
#define CLASS   "CPSC 427a/527a"

#include <stdio.h>
#include <stdlib.h>     /* for malloc() and calloc()*/
#include <string.h>     /* for time_t, time() and ctime()*/
#include <math.h>
#include <time.h>
#include <ctype.h>      /* for isspace() and isdigit() */
#include <limits.h>     /* for INT_MAX */
#include <float.h>      /* for DOUBLE_MAX, etc. */
#include <stdarg.h>     /* for functions with variable # of arguments */
#include <unistd.h>     /* for Unix-specific functions */
#include <stdbool.h>

/* Nonstandard definitions to make C look more like C++ */
//typedef char* string;
//typedef const char* const_string;
//typedef FILE* stream;

/* --------------------------------------------------------------------------- */
/* Routine screen and process management.------------------------------------- */
/* --------------------------------------------------------------------------- */
void    fbanner( FILE* sout );
#define banner()  fbanner( stdout )
void    bye( void );
void    say( const char* format, ... );
void    delay( int );

/* --------------------------------------------------------------------------- */
/* Error handling. ----------------------------------------------------------- */
/* --------------------------------------------------------------------------- */
int     fatal( const char* format, ... );

/* --------------------------------------------------------------------------- */
/* Time and date. -------------------------------------------------------------*/
/* --------------------------------------------------------------------------- */
void    when( char* date, char* hour );
char*  today( char* date );
char*  oclock( char* hour );

#endif
