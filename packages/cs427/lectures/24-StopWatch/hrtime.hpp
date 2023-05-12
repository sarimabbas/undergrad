/*
 * hrtime.hpp
 *
 *  Created on: Oct 4, 2009
 *      Author: Michael Fischer
 *  For use in Yale course CPSC 427a, Fall 2010
 */

#pragma once

#include <iostream>
#include <ctime>
#include <sys/time.h>
using namespace std;

//-------------------------------------------------------------------
// Wrapper class for the darwin clock functions
// described in gettimeofday(2).
// These functions represent time by a struct timeval, which
// consists of a pair of longs representing seconds and microseconds
// (units of 10e-6 seconds).
//-------------------------------------------------------------------

class HirezTime: protected timeval {
	// Internal representation of HirezTime is visible to derived
	// classes but not to public clients.
protected:
	HirezTime(time_t sec, long usec) {
		tv_sec = sec;
		tv_usec = usec;
	}
	// Get functions
	long getSec() 			   const { return tv_sec; }
	long getUsec() 			   const { return tv_usec; }

public:
	HirezTime() { tv_sec = tv_usec = 0L; }

	void reset() { tv_sec = tv_usec = 0L; }
	void readClock();
	HirezTime operator+(HirezTime ht2) const;
	HirezTime operator-(HirezTime ht2) const;
	ostream& print(ostream& os) const;
};
inline ostream& operator<<(ostream& os, const HirezTime& ht) {
	return ht.print(os);
}

