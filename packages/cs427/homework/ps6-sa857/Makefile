# Macros
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = driver.o blockchain.o block.o tools.o SPtr.o Serial.o

# Rules
all: blockchain test
blockchain: main.o $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^
test: test.o $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^
clean:
	rm -f blockchain main.o test test.o $(OBJ)

# Dependencies
main.o: main.cpp driver.hpp blockchain.hpp block.hpp tools.hpp SPtr.hpp Serial.hpp
driver.o: driver.cpp driver.hpp blockchain.hpp block.hpp tools.hpp SPtr.hpp Serial.hpp
blockchain.o: blockchain.cpp blockchain.hpp block.hpp tools.hpp SPtr.hpp Serial.hpp
block.o: block.cpp block.hpp tools.hpp SPtr.hpp Serial.hpp
SPtr.o: SPtr.cpp SPtr.hpp Serial.hpp block.hpp
Serial.o: Serial.cpp Serial.hpp

test.o: test.cpp driver.hpp blockchain.hpp block.hpp tools.hpp SPtr.hpp Serial.hpp