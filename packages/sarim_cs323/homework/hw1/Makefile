CC     = gcc
CFLAGS = -g3 -std=c99 -pedantic -Wall

all: farthing
farthing: farthing.o list.o
	$(CC) -o farthing farthing.o list.o
farthing.o: farthing.c farthing.h list.h
	$(CC) $(CFLAGS) -c farthing.c list.c
list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c


clean:
	-rm *.o farthing 

