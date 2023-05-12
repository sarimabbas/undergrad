/* File: tools.c - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  /
/  The tools library.                                                           /
/  Assorted utility routines for use in C programs.                             /
/  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#include "tools.h"

/* ---------------------------------------------------------------------------- /
/  Routine screen and process management.                                       /
/ ---------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------
** Print a neat header on the output.
*/
void
fbanner( FILE* sout )
{   char date[16], time[9];

    when(date, time);
    fprintf( sout, "\n---------------------------------------------------------------" );
    fprintf( sout, "\n\t%s \n\t%s \n\t%s  %s\n", NAME, CLASS, date, time );
    fprintf( sout, "\n---------------------------------------------------------------\n" );
}

/* ---------------------------------------------------------------------------
**  Print termination message.
*/
void bye( void ) {
    fputs( "\n---------------------------------------------------------------"
           "\nNormal termination.\n", stdout );
}

/* --------------------------------------------------------------------------
** This is a handy function for messages of all sorts.
**  It formats, prints on the standard error stream, and rings the bell.
**  It accepts a format followed by a variable number of data items to print.
*/
void
say (const char* format, ...)
{   va_list vargs;                               /* optional arguments */

    va_start( vargs, format );
    vfprintf( stdout, format, vargs );
    fprintf( stdout, "\n" );
    va_end( vargs );
}

/* --------------------------------------------------------------------------
** Delay progress of program for some number of seconds using a "busy wait".
*/
void
delay( int seconds )
{
    do { seconds = sleep( seconds ); } while (seconds > 0);
}

/* ----------------------------------------------------------------------------/
/  Error handling and error recovery functions.                                /
/-----------------------------------------------------------------------------*/

/* ----------------------------------------------------------------------------
** This function is for error messages.
**    It takes a format argument followed by any number of data arguments.
**    It formats and prints an error message, then exits.                            **
*/
int
fatal( const char* format, ... )
{   va_list vargs;                               /* optional arguments */

    fflush(stdout);
    va_start(vargs, format);
    vfprintf(stdout, format, vargs);
    fprintf(stdout, "\n");
    exit(1);
}

/* ---------------------------------------------------------------------------- /
/  Routines for handling the time and date.                                     /
/ ---------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
** Store the current date and time in the arguments.
**      System's date format is: "Fri Jun  9 10:15:55 1995\n"
**      After extraction, date is: "Fri Jun  9 1995"    hour is: "10:15:55"
*/
void
when( char* date, char* hour )
{
    time_t now;          /* Stores an integer encoding of the date and time. */
    char* nowstring;    /* Stores the date and time in a readable form.     */

    now = time(NULL);              /* Get the date and time from the system. */
    nowstring = ctime(&now);                   /* Convert to string form.    */
    strncpy( date, nowstring, 10);             /* Extract day, month, date.  */
    strncpy( &date[10], &nowstring[19], 5);    /* Extract space and year.    */
    date[15]  = '\0';                          /* Add the string terminator. */
    strncpy( hour, &nowstring[11], 8);         /* Copy hour:minutes:seconds. */
    hour[8]  = '\0';                           /* Add the string terminator. */
}

/* --------------------------------------------------------------------------
** Store the current date in the argument and return a pointer to it.
**      date format is: "Fri Jun  9 1995"
*/
char*
today( char* date )
{
    time_t now;          /* Stores an integer encoding of the date and time. */
    char* nowstring;    /* Stores the date and time in a readable form.     */

    now = time(NULL);              /* Get the date and time from the system. */
    nowstring = ctime(&now);                   /* Convert to string form.    */
    strncpy( date, nowstring, 10);             /* Extract day, month, date.  */
    strncpy( &date[10], &nowstring[19], 5);    /* Extract space and year.    */
    date[15]  = '\0';                          /* Add the string terminator. */
    return( date );
}

/* --------------------------------------------------------------------------
** Store the current time in hour and return a pointer to it.
**      hour format is: "10:15:55"
*/
char*
oclock( char* hour )
{
    time_t now;          /* Stores an integer encoding of the date and time. */
    char* nowstring;    /* Stores the date and time in a readable form.     */

    now = time(NULL);              /* Get the date and time from the system. */
    nowstring = ctime(&now);              /* Convert to string form.         */
    strncpy( hour, &nowstring[11], 8);    /* Extract hour, minutes, seconds. */
    hour[8]  = '\0';                      /* Add the string terminator.      */
    return( hour );
}
