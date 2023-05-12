# Makefile for Consensus2 executable
#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -g -Wall -std=c++17
OBJ = simulator.o population.o fickle.o crowd.o tools.o
#-----------------------------------------------------------
# Rules
all: consensus2 test
consensus2: main.o $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^
test: test.o $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^
clean:
	rm -f main.o test.o consensus2 test $(OBJ)
#-----------------------------------------------------------
# Dependencies
main.o: main.cpp simulator.hpp population.hpp fickle.hpp agent.hpp tools.hpp crowd.hpp
test.o: test.cpp fickle.hpp agent.hpp tools.hpp crowd.hpp population.hpp simulator.hpp

simulator.o: simulator.cpp simulator.hpp population.hpp fickle.hpp agent.hpp crowd.hpp tools.hpp
population.o: population.cpp population.hpp fickle.hpp agent.hpp crowd.hpp tools.hpp

fickle.o: fickle.cpp fickle.hpp agent.hpp tools.hpp
crowd.o: crowd.cpp crowd.hpp agent.hpp tools.hpp

tools.o: tools.cpp tools.hpp
