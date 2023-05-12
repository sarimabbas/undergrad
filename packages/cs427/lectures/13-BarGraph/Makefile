# Author: Michael J. Fischer
# Date: September 2016

#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJBASE = row.o tools.o
OBJ = $(OBJBASE) main.o graph.o
OBJNEST = $(OBJBASE) mainNest.o graphNest.o
TARGET = bar barNest
#-----------------------------------------------------------
# Rules
all: $(TARGET)

bar: $(OBJ)
	$(CXX) -o $@ $(OBJ)

barNest: $(OBJNEST)
	$(CXX) -o $@ $(OBJNEST)

# graphNest.o and mainNest.o are just like graph.o and main.o
# except they are compiled with -DNESTED
graphNest.o: graph.cpp graph.hpp tools.hpp row.hpp item.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ -DNESTED graph.cpp

mainNest.o: main.cpp tools.hpp graph.hpp row.hpp item.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ -DNESTED main.cpp

clean:
	rm -f $(OBJ) $(OBJNEST) $(TARGET)
#-----------------------------------------------------------
# Dependencies
graph.o: graph.cpp graph.hpp tools.hpp row.hpp item.hpp
main.o: main.cpp tools.hpp graph.hpp row.hpp item.hpp
row.o: row.cpp row.hpp item.hpp
tools.o: tools.cpp tools.hpp
