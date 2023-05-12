#-----------------------------------------------------------
# Macro definitions
CC = gcc
CFLAGS = -g3 -std=c99 -pedantic -Wall -I/c/cs323/Hwk5/ -I/c/cs323/Hwk2/
HWK5 = /c/cs323/Hwk5
HWK2 = /c/cs323/Hwk2
#-----------------------------------------------------------
# Rules
all: bashful test
bashful: process.o $(HWK5)/mainBashful.o $(HWK5)/parsnip.o $(HWK2)/tokenize.o
	$(CC) $(CFLAGS) -o $@ $^
test: test.o process.o $(HWK5)/parsnip.o $(HWK2)/tokenize.o
	$(CC) $(CFLAGS) -o $@ $^
clean:
	rm -f bashful test test.o process.o
#-----------------------------------------------------------
# Dependencies
test.o: test.c process-stub.h $(HWK2)/parsnip.h
mainBashful.o: mainBashful.c $(HWK2)/parsnip.h process-stub.h
process.o: process.c process-stub.h $(HWK2)/parsnip.h



