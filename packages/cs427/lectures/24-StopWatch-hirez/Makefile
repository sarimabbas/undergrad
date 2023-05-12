# Author: Michael J. Fischer
# Date: October 24, 2016

#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = main.o hrtime.o tools.o
TARGET = timetest
#-----------------------------------------------------------
# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)
#-----------------------------------------------------------
# Dependencies
hrtime.o: hrtime.cpp tools.hpp hrtime.hpp
main.o: main.cpp tools.hpp stopwatch.hpp hrtime.hpp
tools.o: tools.cpp tools.hpp

