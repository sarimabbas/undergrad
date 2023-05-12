//  ----------------------------------------------------------------------------
//  Header file for all DataPack programs.                          datapack.hpp
//  Created by Alice Fischer on Sunday August 23, 2009
// -----------------------------------------------------------------------------
#pragma once
#include "tools.hpp" // includes all the other headers you need.

typedef float BT;   // Generic base type for data pack; BT must implement <=
#define LENGTH  20

class DataPack {
private:
	// ------------------------------------------------------------------
	int n;      // The current number of items in the array.
	int max;    // Allocation length of the array.
	BT* store;  // For a dynamically allocated data array.

	void read();

public:
	// -------------------------------------------------------------------
	DataPack() {
		n = 0;
		max = LENGTH;
		store = new BT[max];
		cout << "Store allocated.\n";
		// C++ throws an exception if the allocation fails.  No test needed here.
		read();
	}
	~DataPack() {
		delete[] store;
		cout << "Store deallocated.\n";
	}

	// -------------------------------------------------------------------------
	int size() const {
		return n;
	}
	void print(ostream& out) const;
	void sort();
};
