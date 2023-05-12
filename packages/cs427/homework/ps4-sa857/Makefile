# Makefile for Consensus executable
#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = simulator.o agent.o tools.o
#-----------------------------------------------------------
# Rules
all: consensus test
consensus: main.o $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^
test: test.o $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^
clean:
	rm -f main.o test.o consensus test $(OBJ)
#-----------------------------------------------------------
# Dependencies
main.o: main.cpp simulator.hpp agent.hpp tools.hpp
test.o: test.cpp simulator.hpp agent.hpp tools.hpp
simulator.o: simulator.cpp simulator.hpp agent.hpp
agent.o: agent.cpp agent.hpp
tools.o: tools.cpp tools.hpp
