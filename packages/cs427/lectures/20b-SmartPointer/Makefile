# Author: Michael J. Fischer
# Date: October 24, 2016
# Revised: November 11, 2018

#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = main.o Serial.o SPtr.o
TARGET = sptr
#-----------------------------------------------------------
# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)
#-----------------------------------------------------------
# Dependencies
main.o: main.cpp SPtr.hpp Serial.hpp
Serial.o: Serial.cpp Serial.hpp
SPtr.o: SPtr.cpp SPtr.hpp Serial.hpp
