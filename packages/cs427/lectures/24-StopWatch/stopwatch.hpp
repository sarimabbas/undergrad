/*
 * stopwatch.hpp
 *
 *  Created on: Oct 4, 2009
 *      Author: mike
 *  For use in Yale course CPSC 427a, Fall 2010
 */
#pragma once

#include <iostream>
#include "tools.hpp"
#include "hrtime.hpp"
using namespace std;

//-------------------------------------------------------------------
class StopWatch {
private:
	const HirezTime zero;
	HirezTime startTime;
	HirezTime stopTime;
	HirezTime cumSpan; // cumulative time span
	bool running;

public:
	StopWatch() {
	}
	void reset() {
		startTime = stopTime = cumSpan = zero;
		running = false;
	}
	void start() {
		if (running)
			throw Fatal("Stopwatch already running");
		startTime.readClock();
		running = true;
	}
	void stop() {
		if (!running)
			throw Fatal("Stopwatch already stopped");
		stopTime.readClock();
		running = false;
		cumSpan = cumSpan + (stopTime - startTime);
	}

	// Cast cumulative time span to a HirezTime.
	// Eliminates need for separate get and print methods.
	operator HirezTime() const {
		return cumSpan;
	}
};
