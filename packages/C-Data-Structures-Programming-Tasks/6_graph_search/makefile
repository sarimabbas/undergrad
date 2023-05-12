# This makefile was adapted from Prof. Aspnes notes

# We STRONGLY recommend putting "-Wall -pedantic -g3" in your CFLAGS.
CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -g3

all: Paths Search Unit

Paths: paths.o ldigraph.o queue.o
	$(CC) $(CFLAGS) -o $@ $^

Search: search.o ldigraph.o queue.o
	$(CC) $(CFLAGS) -o $@ $^

Unit: unit.o ldigraph.o queue.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o *.h.gch Paths Search Unit
