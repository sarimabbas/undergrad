/*
 * main.cpp
 *
 *  Created on: Oct 3, 2009; modified Oct 19, 2010
 *      Author: Michael Fischer
 *  For use in Yale course CPSC 427a, Fall 2010
 */

//-------------------------------------------------------------------
// Test program for StopWatch class.
// This program generates 1,000,000 random numbers and stores them
// in a list.
// It first uses an array allocated from the heap.
// It then uses a linked list.
// It prints out the elapsed CPU time for each method.
//-------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "stopwatch.hpp"
#include "tools.hpp"

using namespace std;

#define ITERATIONS 20000000 // 20 million
//-------------------------------------------------------------------
class Cell {
public:
	int data; // data item
	Cell* next; // pointer to next cell
	Cell(int s, Cell* nx) {
		data = s;
		next = nx;
	}
};

//-------------------------------------------------------------------
int main(void) {
	banner();
	cout << "StopWatch test\n" << endl;

	StopWatch sw;
	cout << "Testing lists with " << ITERATIONS << " numbers\n" << endl;
	//-------------------------------------------------------------------
	// Test 1:  Array list
	sw.reset();
	sw.start();
	int* array = new int[ITERATIONS];
	for (int k = 0; k < ITERATIONS; k++) {
		array[k] = rand();
		// array[k] = 123;  // Much time is spent in rand(); try with constants
	}
	delete[] array;
	sw.stop();
	cout << "Array list:  " << sw << " seconds" << endl;

	//-------------------------------------------------------------------
	// Test 2: Linked list
	sw.reset();
	sw.start();
	Cell* head = nullptr;
	for (int k = 0; k < ITERATIONS; k++) {
		head = new Cell(rand(), head);
		// head = new Cell(123, head);
	}
	while (head != nullptr) {
		Cell* p = head;
		head = head->next;
		delete p;
	}
	sw.stop();
	cout << "Linked list: " << sw << " seconds" << endl;
	bye();
}
