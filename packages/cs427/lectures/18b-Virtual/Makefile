# Author: Michael J. Fischer
# Date: November 8, 2016

#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = main.o linear.o
TARGET = contain
#-----------------------------------------------------------
# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)
#-----------------------------------------------------------
# Dependencies
linear.o: linear.cpp linear.hpp container.hpp item.hpp exam.hpp cell.hpp
main.o: main.cpp exam.hpp stack.hpp linear.hpp container.hpp item.hpp \
 cell.hpp queue.hpp
