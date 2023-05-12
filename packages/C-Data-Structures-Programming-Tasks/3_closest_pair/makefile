# This makefile was adapted from Prof. Aspnes notes

# We STRONGLY recommend putting "-Wall -pedantic -g3" in your CFLAGS.
CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -g3 -lm

all: ClosestPair

Unit: plist_unit.o plist.o point.o
	$(CC) $(CFLAGS) -o $@ $^

ClosestPair: closest_pair.o plist.o point.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o *.h.gch Unit ClosestPair
