/*
 * main.cpp
 *
 *  Created on: Sep 10, 2010
 *      Author: fischer
 *  Modified by Michael Fischer, September 7, 2016
 */

#include "tools.hpp" // includes all the other headers you need.
typedef float BT; // Generic base type for data pack; BT must implement <=
#define LENGTH  20

// ----------------------------------------------------------------------------
int main(void) {
	int n; // The current number of items in the array.
	int max; // Allocation length of the array.
	BT* store; // For a dynamically allocated data array.
	char filename[80]; // For name of input file, 79 char max
	BT* end;
	BT* pass; // First unsorted item; begin pass here.
	BT newcomer; // Data value being inserted.
	BT* hole; // Array slot containing no data.

	banner();
	cout << "Construct pack, read data";

	n = 0;
	max = LENGTH;
	store = new BT[max];
	cout << "Store allocated.\n";

	cout << "Enter name of data file to be searched: ";
	cin.getline(filename, 80);
	ifstream infile(filename);
	if (!infile)
		fatal("    Error: couldn't open input %s\n", filename);

	BT* cursor = store; // Scanning pointer, set to start of array.
	end = store + max; // An off-board sentinel

	for (; cursor < end; ++cursor) {
		infile >> *cursor;
		if (!infile.good())
			break; // Quit for bad data or for end of file.
	}
	n = cursor - store; // Actual # of items read.

	cout << "\n" << n << " data items read:" << n;
	cout << "\nUnsorted data:\n";
	for (int k = 0; k < n; ++k) {
		cout << store[k] << "\n";
	}

	cout << "\nBeginning to sort.";

	end = store + n; // Off-board sentinel.
	for (pass = &store[1]; pass < end; ++pass) {
		// Pick up next item and insert into sorted portion of array.
		newcomer = *pass;
		for (hole = pass; hole > store; --hole) {
			if (*(hole - 1) <= newcomer)
				break; // Insertion slot is found.
			*hole = *(hole - 1); // Move item 1 slot rightward.
		}
		*hole = newcomer;
	}

	cout << "\nSorted results:\n";
	for (int k = 0; k < n; ++k) {
		cout << store[k] << "\n";
	}

	infile.close();

	delete[] store;
	cout << "Store deallocated.\n";

	bye();
	return 0;
}
