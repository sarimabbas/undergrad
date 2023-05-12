# Author: Michael J. Fischer
# Date: September 4, 2012
# Modified: September 4, 2018

CFLAGS = -O1 -g -Wall -std=c17
OBJ = main.o datapack.o tools.o
TARGET = isort

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)

# Dependencies
datapack.o: datapack.c datapack.h tools.h
main.o: main.c datapack.h tools.h
tools.o: tools.c tools.h
