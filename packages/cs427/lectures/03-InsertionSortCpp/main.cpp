//  ---------------------------------------------------------------------------
//  Main program for Sorting DataPacks.                                main.cpp
//  Created by Alice Fischer on Mon August 23, 2009
//  Modified by Michael Fischer on September 4, 2018
// ----------------------------------------------------------------------------
#include "datapack.hpp"
#include "tools.hpp"

void run();

// ----------------------------------------------------------------------------
int main() {
	banner();
	run();
	bye();
	return 0;
}

void run() {
	cout << "Construct pack, read data";
	DataPack theData;
	cout << "\n" << theData.size() <<" data items read:";
	cout << "\nUnsorted data:\n";
	theData.print(cout);
	cout << "\nBeginning to sort.";
	theData.sort();
	cout << "\nSorted results:\n";
	theData.print(cout);
}
