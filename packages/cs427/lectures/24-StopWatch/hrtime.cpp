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
// Read system clock
void HirezTime::readClock() {
	int ret = gettimeofday(this, nullptr);
	if (ret<0) perror("Error reading clock");
}

//-------------------------------------------------------------------
// Operator extension to add two HirezTime values
HirezTime
HirezTime::operator+(HirezTime ht2) const {
	long sum_sec = getSec() + ht2.getSec();
	long sum_usec = getUsec() + ht2.getUsec();
	if (sum_usec >= 1000000L) {
		sum_usec -= 1000000L;
		sum_sec++;
	}
	return HirezTime(sum_sec, sum_usec);
}

//-------------------------------------------------------------------
// Operator extension to subtract two HirezTime values
// Assumes first operand >= second, so difference is non-negative.
HirezTime
HirezTime::operator-(HirezTime ht2) const {
	long diff_sec = getSec() - ht2.getSec();
	long diff_usec = getUsec() - ht2.getUsec();
	if (diff_sec < 0 || (diff_sec == 0 && diff_usec < 0))
		throw Fatal("Can't handle negative time differences");
	if (diff_usec < 0) {
		diff_usec += 1000000L;
		diff_sec--;
	}
	return HirezTime(diff_sec, diff_usec);
}

//-------------------------------------------------------------------
// Prints HirezTime value in seconds as a decimal fraction.
ostream&
HirezTime::print(ostream& os) const {
	char prev = cout.fill('0');
	os<< getSec()<< "."<< right<< setw(6)<< getUsec();
	cout.fill(prev);
	return os;
}

