/*
 * main.cpp
 *
 *  Created on: Feb 25, 2016
 *      Author: mike
 */

#include <iostream>
#include "SPtr.hpp"

//-------------------------------------------------------
int main() {
try {
    cout << "Create smart pointer sp using default constructor" << endl;
    SPtr sp;
    if (sp.empty()) cout << "  Smart pointer " << sp.id() << " is empty" << endl;

    cout << "\nCreate new double variable xp" << endl;
    T* xp = new T(12.345);
    cout << "  *xp = " << *xp << endl;

    cout << "\nCreate temporary smart pointer to manage xp\n"
            "  and move assign it to sp" << endl;
    sp = SPtr(xp);    // move new SPtr to sp
    cout << "  *sp = " << *sp << endl;

    cout << "\nCreate copy of SPtr " << sp.id() << endl;
    SPtr sp2(sp);// make a copy
    cout << "  Copy is SPtr " << sp2.id() << endl;

    cout << "\nAssigning *sp2 = 67.890;" << endl;
    *sp2 = 67.890;
    cout << "  *sp = " << *sp << endl;
    cout << "  *sp2 = " << *sp2 << endl;

    cout << "\nLeaving main program" << endl;
}
catch (SPexception &e ) {
    cerr << e.what() << endl;
    throw;
}
}

