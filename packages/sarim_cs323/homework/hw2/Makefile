CC     = gcc
CFLAGS = -g3 -std=c99 -pedantic -Wall -I/c/cs323/Hwk2/
HWK = /c/cs323/Hwk2

all: parsnip

parsnip: parse.o ${HWK}/mainParsnip.o ${HWK}/tokenize.o
	$(CC) -o parsnip parse.o ${HWK}/mainParsnip.o ${HWK}/tokenize.o
# mainParsnip.o: mainParsnip.c parsnip.h
# 	$(CC) $(CFLAGS) -c mainParsnip.c
parse.o: parse.c parse.h ${HWK}/parsnip.h 
	$(CC) $(CFLAGS) -c parse.c

clean:
	-rm parsnip parse.o 

