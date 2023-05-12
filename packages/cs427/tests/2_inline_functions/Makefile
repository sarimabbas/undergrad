#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
COMMON_OBJ = flipflop.o tools.o
TEST_OBJ = test.o $(COMMON_OBJ)
TARGET = test
#-----------------------------------------------------------
# Rules
all: test
test: $(TEST_OBJ)
	$(CXX) -o $@ $(TEST_OBJ)
clean:
	rm -f $(COMMON_OBJ) tad.o test.o $(TARGET)
#-----------------------------------------------------------
# Dependencies
test.o: test.cpp tools.hpp flipflop.hpp
flipflop.o: flipflop.cpp flipflop.hpp
tools.o: tools.cpp tools.hpp
