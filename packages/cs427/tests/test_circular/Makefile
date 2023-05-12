# Macros
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = a.o b.o

# Rules
all: test 
test: main.o $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^
clean:
	rm -f a.o b.o test main.o

# Dependencies
main.o: main.cpp a.hpp b.hpp
a.o: a.cpp a.hpp b.hpp 
b.o: b.cpp a.hpp b.hpp 

