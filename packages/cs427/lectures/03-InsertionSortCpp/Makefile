# Author: Michael J. Fischer
# Date: September 4, 2018

#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = main.o datapack.o tools.o
TARGET = isort
#-----------------------------------------------------------
# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)
#-----------------------------------------------------------
# Dependencies
datapack.o: datapack.cpp datapack.hpp tools.hpp
main.o: main.cpp datapack.hpp tools.hpp
tools.o: tools.cpp tools.hpp
