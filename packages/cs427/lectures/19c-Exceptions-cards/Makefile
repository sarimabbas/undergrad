# Author: Michael J. Fischer
# Date: November 16, 2016

#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = cards.o except.o
TARGET = cards
#-----------------------------------------------------------
# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)
#-----------------------------------------------------------
# Dependencies
cards.o: cards.cpp cards.hpp bad.hpp
except.o: except.cpp cards.hpp bad.hpp
