#include <iostream>

class Base {
public:
	virtual ~Base() {}
	virtual void dummy() {
	}
};

class Derived1: public Base {
};

class Derived2: public Base {
};

int main() {
	// objects
	Base base;
	Derived1 derived1;
	Derived2 derived2;

	// pointers
	Base* pbase;
	Derived1* pderived1;

	// pbase points to Derived1 object
	pbase = &derived1; // ok: derived-to-base

	pderived1 = dynamic_cast<Derived1*> (pbase);

	std::cout << "Casting from Derived1 to Derived1: ";
	if (pderived1)
		std::cout << "dynamic_cast succeeded" << std::endl;
	else
		std::cout << "dynamic_cast failed" << std::endl;

	// pbase points to Derived2 object;
	// attempt to dynamic cast to Derived1 object
	pbase = &derived2; // ok: derived-to-base

	pderived1 = dynamic_cast<Derived1*> (pbase);

	std::cout << "Casting from Derived2 to Derived1: ";
	if (pderived1)
		std::cout << "dynamic_cast succeeded" << std::endl;
	else
		std::cout << "dynamic_cast failed" << std::endl;
}
