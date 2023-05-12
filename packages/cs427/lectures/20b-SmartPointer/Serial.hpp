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
   static Serial* Sobj;
   int nextUID=0;
   Serial() =default;

public:
   static Serial& uidGen() {
      if (Sobj == nullptr) Sobj = new Serial;
      return *Sobj;
   }
   const int operator()() { return nextUID++; }
};

#endif /* SERIAL_HPP_ */
