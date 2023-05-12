#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

//--------------------------------------------------------------------
bool descendingOrder( int i, int j) { return (i>j); }
static bool ascendingOrder( int i, int j)  { return (i<j); }

//--------------------------------------------------------------------
void print(vector<int>& vec) {
   cout << "myvector contains:";
   for (vector<int>::iterator it=vec.begin(); it!=vec.end(); ++it)
      cout << ' ' << *it;
   cout << '\n';
}

//--------------------------------------------------------------------
int main() {
   
    // Test vector
    int myints[] = {32,71,12,45,26,80,53,33};
    vector<int> myvector (myints, myints+8); // 32 71 12 45 26 80 53 33

    // Define expected type for a sort comparator
    // using Comp = bool (*) (int, int);
    typedef bool (*Comp) (int, int);

    // Define a pointer to a comparator
    // Comp cptr;

    // Set pointer and sort descending
    Comp cptr = descendingOrder;
    sort( myvector.begin(), myvector.end(), ascendingOrder );
    print( myvector );

    // Set pointer and sort ascending
    cptr = ascendingOrder;
    sort ( myvector.begin(), myvector.end(), cptr );
    print( myvector );    
}
