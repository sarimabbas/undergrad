/*
 * hrtime.cpp
 *
 *  Created on: Oct 4, 2009
 *      Author: Michael Fischer
 *  For use in Yale course CPSC 427a, Fall 2010
 */

#include "tools.hpp"
#include "hrtime.hpp"

//-------------------------------------------------------------------
// Read a system clock specified by whichClock.
// This should be one of the clocks
// described by clock_gettime(2).
// Supported values on many linux systems are:
//   CLOCK_REALTIME				System-wide realtime clock.
//   CLOCK_MONOTONIC			Represents monotonic time.
//   CLOCK_PROCESS_CPUTIME_ID	High resolution per-process CPU timer.
//   CLOCK_THREAD_CPUTIME_ID	Thread-specific CPU-time clock.
void HirezTime::readClock(clockid_t whichClock) {
	int ret = clock_gettime(whichClock, this);
	if (ret<0) perror("Error reading clock");
}

//-------------------------------------------------------------------
// Operator extension to add two HirezTime values
HirezTime
HirezTime::operator+(HirezTime ht2) const {
	long sum_sec = getSec() + ht2.getSec();
	long sum_nsec = getNsec() + ht2.getNsec();
	if (sum_nsec >= 1000000000L) {
		sum_nsec -= 1000000000L;
		sum_sec++;
	}
	return HirezTime(sum_sec, sum_nsec);
}

//-------------------------------------------------------------------
// Operator extension to subtract two HirezTime values
// Assumes first operand >= second, so difference is non-negative.
HirezTime
HirezTime::operator-(HirezTime ht2) const {
	long diff_sec = getSec() - ht2.getSec();
	long diff_nsec = getNsec() - ht2.getNsec();
	if (diff_sec < 0 || (diff_sec == 0 && diff_nsec < 0))
		throw Fatal("Can't handle negative time differences");
	if (diff_nsec < 0) {
		diff_nsec += 1000000000L;
		diff_sec--;
	}
	return HirezTime(diff_sec, diff_nsec);
}

//-------------------------------------------------------------------
// Prints HirezTime value in seconds as a decimal fraction.
ostream&
HirezTime::print(ostream& os) const {
	char prev = cout.fill('0');
	os<< getSec()<< "."<< right<< setw(9)<< getNsec();
	cout.fill(prev);
	return os;
}

