# This makefile was adapted from Prof. Aspnes notes

# We STRONGLY recommend putting "-Wall -pedantic -g3" in your CFLAGS.
CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -g3

all: Grundy Test

Grundy: grundy.o isset.o
	$(CC) $(CFLAGS) -o $@ $^

Test: isset_test.o isset.o
	$(CC) $(CFLAGS) -o $@ $^

#AwesomeTest: awesome_test.o isset.o
#	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o *.h.gch Grundy Test 
