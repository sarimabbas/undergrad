#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

//--------------------------------------------------------------------
bool descendingOrder( int i, int j) { return (j<i); }
bool ascendingOrder( int i, int j)  { return (i<j); }

//--------------------------------------------------------------------
void print(vector<int>& vec) {
    cout << "myvector contains:";
    for (vector<int>::iterator it=vec.begin(); it!=vec.end(); ++it)
        cout << ' ' << *it;
    cout << '\n';
}

//--------------------------------------------------------------------
class ComparDescending {
public:
    bool operator()(int i, int j) { return (i>j); }
};

//--------------------------------------------------------------------
class ComparAscending {
public:
    bool operator()(int i, int j) { return (i<j); }
};

//--------------------------------------------------------------------
int main() {

    int myints[] = {32,71,12,45,26,80,53,33};
    vector<int> myvector (myints, myints+8); // 32 71 12 45 26 80 53 33

    sort ( myvector.begin(), myvector.end(), ComparDescending() );
    print( myvector );

    sort ( myvector.begin(), myvector.end(), ComparAscending() );
    print( myvector );
}
