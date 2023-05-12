//                  -*- mode:c++; tab-width:4 -*-
// File: main.cpp ----------------------------------------------------------
// Implementation files for aboutme app.
// Author: Sarim Abbas.
// Created September 2018. Modified September 2018.
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "tools.hpp"

void run( void );

// ---------------------------------------------------------------------------
//  Calls functions to show a bannner, collect user info, and exit.

int main() {  
    banner();  
    run();  
    bye();  
}

// ---------------------------------------------------------------------------
//  Collect input from user and display this input and user's age.

void run( void ) {
    // get user input
    string first_name, last_name;
    int year_of_birth;
	cout << "Please enter your first name: ";
	cin >> first_name;
	cout << "Please enter your last name: ";
	cin >> last_name;
    cout << "Please enter the year of your birth: ";

    // year of birth validation (see notes.txt)
    while(!(cin >> year_of_birth)) {
        cin.clear();
        cin.ignore( numeric_limits<streamsize>::max(), '\n' );
        cout << "Please enter an integer for your year of birth: ";
    }
     
    // compute age
    time_t current_time = time( nullptr );
    struct tm *time_buf = localtime( &current_time );
    int current_year = time_buf->tm_year + 1900;
    int age = current_year - year_of_birth;

    // output information
    cout << first_name << " " << last_name << " becomes " 
    << age << " years old in " << current_year << ".";
}   