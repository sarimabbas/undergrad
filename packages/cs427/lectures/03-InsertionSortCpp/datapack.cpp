// ----------------------------------------------------------------------------- 
//  DataPack class:  an array plus its management information.
//  Implementation file for all DataPack programs.                  datapack.cpp
//  Created by Alice Fischer on Mon August 23, 2009
//  Modified by Michael Fischer, September 7, 2016
// ----------------------------------------------------------------------------- 
#include "datapack.hpp"

// --------------------------------------------------------------------- 
//  Read (from user) name of input file and open it.  
//  Read up to max items from input stream and store in array. 
void DataPack::read() {
	char filename[80];      // For name of input file, 79 char max
	cout << "Enter name of data file to be searched: ";
	cin.getline(filename, 80);
	ifstream infile(filename);
	if (!infile)
        	fatal("Error: couldn't open input file '%s'\n", filename);

	BT* cursor = store;     // Scanning pointer, set to start of array.
	BT* end = store + max;  // An off-board sentinel
	BT item;				// Read buffer
	for (;;) {
		infile >> item;				// Try to get next item from file
		if (infile.fail())
			break;	// Illegal/missing data or read error
		if (cursor == end) {			// Good data, but nowhere to put it
			fatal("DataPack too small to contain entire file");
		}
		*cursor++ = item;			// Save item to store
	}
	if (!infile.eof())  // Reading stopped before EOF
		fatal("Error reading input file %s", filename);
	n = cursor - store;                   // Actual # of items read.
	infile.close();
}

// --------------------------------------------------------------------- 
//  Print the data from the array. 
void DataPack::print(ostream& out) const {
	for (int k = 0; k < n; ++k) {
		out << store[k] << "\n";
	}
}

// --------------------------------------------------------------------- 
//  Sort data in the data pack.  The < and <= operators must be defined for BT.  
//
void DataPack::sort() {
	BT* const end = store + n;  // Off-board sentinel.
	BT* pass;                   // First unsorted item; begin pass here.
	BT newcomer;               // Data value being inserted.
	BT* hole;                   // Array slot containing no data.

	for (pass = &store[1]; pass < end; ++pass) {
		// Pick up next item and insert into sorted portion of array.
		newcomer = *pass;
		for (hole = pass; hole > store; --hole) {
			if (*(hole - 1) <= newcomer)
				break;   // Insertion slot is found.
			*hole = *(hole - 1);                // Move item 1 slot rightward.
		}
		*hole = newcomer;
	}
}
