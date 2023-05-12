# This makefile was adapted from Prof. Aspnes notes

# We STRONGLY recommend putting "-Wall -pedantic -g3" in your CFLAGS.
CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -g3

all: WordCount Cooccur CooccurUnit SmapUnit

Cooccur: cooccur_exec.o cooccur.o smap.o
	$(CC) $(CFLAGS) -o $@ $^

coccur.o: cooccur.c smap.h
	$(CC) $(CFLAGS) -c $@ $^

WordCount: word_count.o smap.o
	$(CC) $(CFLAGS) -o $@ $^

# it turns out that even though Unit needs cooccur.h, using cooccur.o will not work
# because cooccur.o uses cooccur.c which has a main as well
# so we can just #include in unit.c and everything works
#Unit: unit.o smap.o
#	$(CC) $(CFLAGS) -o $@ $^

CooccurUnit: cooccur_unit.c cooccur.o smap.o smap_test_functions.o
	$(CC) $(CFLAGS) -o $@ $^

SmapUnit: smap_unit.c smap.o smap_test_functions.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o *.h.gch Cooccur WordCount SmapUnit CooccurUnit
