# This makefile was adapted from Prof. Aspnes notes

# We STRONGLY recommend putting "-Wall -pedantic -g3" in your CFLAGS.
CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -g3

all: Chomp SarimTest

Chomp: chomp_main.o chomp.o string_helpers.o smap.o
	$(CC) $(CFLAGS) -o $@ $^

SarimTest: sarim_test.o chomp.o string_helpers.o smap.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o *.h.gch Chomp
