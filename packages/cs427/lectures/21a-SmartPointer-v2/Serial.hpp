/*
 * Serial.hpp
 *
 *  Created on: Nov 11, 2018
 *      Author: mike
 */

#ifndef SERIAL_HPP_
#define SERIAL_HPP_

// ------------------------------------------------------------------------------
// Singleton class for generating unique ID's

class Serial {
private:
   static Serial* Sobj; // pointer to singleton Serial object
   int nextUID=0;       // data member for next UID to be assigned
   static Serial* uidGen() {  // instaniates Serial on first call
      if (Sobj == nullptr) Sobj = new Serial;
      return Sobj;
   }
   Serial() =default;   // private constructor prevents external instantiation
public:
   static int newID() { return uidGen()->nextUID++; }
};

#endif /* SERIAL_HPP_ */
