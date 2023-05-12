#include <iostream>
#include <algorithm>
#include <functional>
#include <typeinfo>
#include <vector>
using namespace std;

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

    // Sort descending using lambda expression comparator
    sort (myvector.begin(), myvector.end(),
          [] (int i, int j) { return (i>j); } );
    print( myvector );

    // Sort ascending using lambda expression comparator
    sort (myvector.begin(), myvector.end(),
          [] (int i, int j) { return (i<j); } );
    print( myvector );
}
