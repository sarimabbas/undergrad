# Author: Michael J. Fischer
# Date: December 12, 2016

#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
TARGET = funtypes using sort-functor sort-funptr \
	 sort-lambda closure
#-----------------------------------------------------------
# Rules
all: $(TARGET)
funtypes : funtypes.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
using : using.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
sort-functor : sort-functor.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
sort-funptr : sort-funptr.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
sort-lambda : sort-lambda.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
closure : closure.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f  $(TARGET)
#-----------------------------------------------------------

