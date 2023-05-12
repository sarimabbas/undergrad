#include <iostream>
#include <iomanip>
#include <functional>
#include <map>
using namespace std;

//--------------------------------------------------------------------
// Sales tax computation
int main() {

    using taxFunType = function<double(double)>;
    map<string,taxFunType> taxTable;
    map<string,taxFunType>::iterator it;

    // Read tax table
    cout << "Enter state sales tax rates.  Type \". 0\" when done." << endl;
    for (;;) {
        string state;
        double rate;

        // Create tax computation function
        cout << "Please enter state and tax rate: ";
        cin >> state >> rate;
        if (!cin.good() || state==".") break;
    
        // Enter it ito the tax rate table    
        taxFunType taxfun = [rate](double amt) { return amt*rate; };
        taxTable.emplace(state, taxfun);
    }
    if (!cin.good()) { cout << "aborting" << endl; return 1; }
    
    // Compute sales tax on individual sales
    cout << "\nEnter individual sales.  Type \". 0\" when done." << endl;
    cin.clear();
    for (;;) {
        string state;
        double sale;
        double tax;
        
        // Compute sales tax
        cout << "\nPlease enter state and purchase amount: ";
        cin >> state >> sale;
        if (!cin.good() || state==".") break;
        it = taxTable.find(state);
        if (it == taxTable.end()) {
            cout << "Can't find " << state << " in tax table" << endl;
        }
        else {
            taxFunType& computeTax = it->second;  // get tax computer from tax table
            tax = computeTax(sale);               // compute tax
            cout << setprecision(2) << fixed;
            cout << state << " tax on $" << sale << " is $" << tax << endl;
        }
    }
    cout << "Goodbye" << endl;
}

/*
Enter state sales tax rates.  Type ". 0" when done.
Please enter state and tax rate: CT 0.0635
Please enter state and tax rate: NY 0.04
Please enter state and tax rate: NJ 0.07
Please enter state and tax rate: MA 0.0625
Please enter state and tax rate: . 0

Enter individual sales.  Type ". 0" when done.

Please enter state and purchase amount: CT 100
CT tax on $100.00 is $6.35

Please enter state and purchase amount: MA 24.95
MA tax on $24.95 is $1.56

Please enter state and purchase amount: NJ 24.95
NJ tax on $24.95 is $1.75

Please enter state and purchase amount: NY 24.95
NY tax on $24.95 is $1.00

Please enter state and purchase amount: PA 24.95
Can't find PA in tax table

Please enter state and purchase amount: CT 24.95
CT tax on $24.95 is $1.58

Please enter state and purchase amount: . 0
Goodbye
*/
