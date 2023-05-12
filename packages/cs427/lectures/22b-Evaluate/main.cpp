// ==========================================================================
// Template example -- Using a stack template.
// Alice E. Fischer June 9, 200file: Evaluate/main.cpp
//
#include "eval.hpp"
// ==========================================================================
int main( void )
{
    char buf[256] = "Hello";
    double answer = 0;

    banner();
    Eval::instructions();
    for(;;){
        cout <<"\n\nEnter an expression: ";
        cin >> ws;
        cin.get( buf, 256 );
        if ( buf[0] == ';') break;
        istringstream inst( buf);
        Eval E;
        answer = E.evaluate( inst );
        cout <<'\n' << answer <<" = " <<buf <<endl;
    }
    bye();
    return 0;
}
