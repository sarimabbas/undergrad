# Author: Michael J. Fischer
# Date: November 14, 2016

#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = eval.o main.o tools.o
TARGET = calc
#-----------------------------------------------------------
# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)
#-----------------------------------------------------------
# Dependencies
eval.o: eval.cpp eval.hpp tools.hpp stackT.hpp flexT.hpp operator.hpp
main.o: main.cpp eval.hpp tools.hpp stackT.hpp flexT.hpp operator.hpp
tools.o: tools.cpp tools.hpp
