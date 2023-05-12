#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O1 -g -Wall -std=c++17
COMMON_OBJ = game.o thinkadot.o flipflop.o tools.o
TAD_OBJ = tad.o $(COMMON_OBJ)
TEST_OBJ = test.o $(COMMON_OBJ)
TARGET = tad test
#-----------------------------------------------------------
# Rules
all: tad test
tad: $(TAD_OBJ)
	$(CXX) -o $@ $(TAD_OBJ)
test: $(TEST_OBJ)
	$(CXX) -o $@ $(TEST_OBJ)
clean:
	rm -f $(COMMON_OBJ) tad.o test.o $(TARGET)
#-----------------------------------------------------------
# Dependencies
tad.o: tad.cpp tools.hpp game.hpp
test.o: test.cpp tools.hpp flipflop.hpp thinkadot.hpp game.hpp
game.o: game.cpp thinkadot.hpp
thinkadot.o: thinkadot.cpp thinkadot.hpp flipflop.hpp
flipflop.o: flipflop.cpp flipflop.hpp
tools.o: tools.cpp tools.hpp
