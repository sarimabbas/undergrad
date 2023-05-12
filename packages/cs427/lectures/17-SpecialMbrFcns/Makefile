# Author: Michael J. Fischer
# Date: September 2016
# Revised: October 2018

#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJBASE = row.o tools.o
OBJ = special.o
TARGET = special
#-----------------------------------------------------------
# Rules
all: $(TARGET)

special: $(OBJ)
	$(CXX) -o $@ $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)
#-----------------------------------------------------------
# Dependencies
special.o: special.cpp
