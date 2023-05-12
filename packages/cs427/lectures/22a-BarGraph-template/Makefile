# Author: Michael J. Fischer
# Date: November 14, 2016

#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = graph.o graphM.o tools.o
TARGET = bargraph
#-----------------------------------------------------------
# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)
#-----------------------------------------------------------
# Dependencies
graph.o: graph.cpp graph.hpp tools.hpp rowNest.hpp item.hpp
graphM.o: graphM.cpp tools.hpp graph.hpp rowNest.hpp item.hpp
tools.o: tools.cpp tools.hpp
