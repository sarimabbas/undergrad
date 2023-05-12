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
using namespace std;

//-------------------------------------------------------------------
// Wrapper class for the high resolution linux clock functions
// described in time(7) and clock_gettime(2).
// These functions represent time by a struct timespec, which
// consists of a pair of longs representing seconds and nanoseconds
// (units of 10e-9 seconds).
//-------------------------------------------------------------------

class HirezTime : protected timespec {
// Internal representation of HirezTime is visible to derived
// classes but not to public clients.
protected:
	HirezTime(time_t sec, long nsec) {
		tv_sec = sec;
		tv_nsec = nsec;
	}
	// Get functions
	long getSec()              const { return tv_sec; }
	long getNsec()             const { return tv_nsec; }

public:
	HirezTime(){ tv_sec = tv_nsec = 0L; }

	void reset() { tv_sec = tv_nsec = 0L; }
	void readClock(clockid_t which_clock);
	HirezTime operator+(HirezTime ht2) const;
	HirezTime operator-(HirezTime ht2) const;
	ostream& print(ostream& os) const;
};
inline ostream& operator<< (ostream& os, const HirezTime& ht) {
	return ht.print(os); }

