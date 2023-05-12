# Define flags for compile.
CXXFLAGS = -g -O1 -Wall -std=c++17

#CXX = clang++
CXX = g++
OBJ = main.o flex.o tools.o

all: FlexArray

FlexArray: $(OBJ)
	$(CXX) -o $@ $(OBJ)

clean:
	rm -f $(OBJ) 

#------------------------------
flex.o: flex.cpp flex.hpp tools.hpp item.hpp
main.o: main.cpp tools.hpp flex.hpp item.hpp
tools.o: tools.cpp tools.hpp
