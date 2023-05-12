# Macros
CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = blockchain.o block.o tools.o SPtr.o Serial.o \
	  agentbase.o multiagent.o population.o simulator.o \
	  inventory.o

# Rules
all: blockchain test

blockchain: main.o $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

test: test.o driver.o $(OBJ) 
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f blockchain test main.o test.o driver.o $(OBJ)

# Dependencies
main.o: main.cpp \
		simulator.hpp population.hpp multiagent.hpp agentbase.hpp agent.hpp \
		blockchain.hpp block.hpp tools.hpp SPtr.hpp Serial.hpp \
		inventory.hpp

tools.o: tools.cpp tools.hpp

test.o: test.cpp \
		simulator.hpp population.hpp multiagent.hpp agentbase.hpp agent.hpp \
		driver.hpp blockchain.hpp block.hpp tools.hpp SPtr.hpp Serial.hpp \
		inventory.hpp

# PS8
inventory.o: inventory.cpp \
			 inventory.hpp blockchain.hpp block.hpp tools.hpp SPtr.hpp Serial.hpp

# PS5
simulator.o: simulator.cpp \
			 simulator.hpp population.hpp multiagent.hpp agentbase.hpp agent.hpp \
			 blockchain.hpp block.hpp tools.hpp SPtr.hpp Serial.hpp \
			 inventory.hpp

population.o: population.cpp \
			  population.hpp multiagent.hpp agentbase.hpp agent.hpp \
			  blockchain.hpp block.hpp tools.hpp SPtr.hpp Serial.hpp

multiagent.o: multiagent.cpp \
			  multiagent.hpp agentbase.hpp agent.hpp \
			  blockchain.hpp block.hpp tools.hpp SPtr.hpp Serial.hpp

agentbase.o: agentbase.cpp \
			 agentbase.hpp agent.hpp \
			 blockchain.hpp block.hpp tools.hpp SPtr.hpp Serial.hpp

# PS6
driver.o: driver.cpp driver.hpp blockchain.hpp block.hpp tools.hpp SPtr.hpp Serial.hpp
blockchain.o: blockchain.cpp blockchain.hpp block.hpp tools.hpp SPtr.hpp Serial.hpp
block.o: block.cpp block.hpp tools.hpp SPtr.hpp Serial.hpp
SPtr.o: SPtr.cpp SPtr.hpp Serial.hpp block.hpp
Serial.o: Serial.cpp Serial.hpp

