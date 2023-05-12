#-----------------------------------------------------------
# Macro definitions
CC = gcc
CFLAGS = -g3 -std=c99 -pedantic -Wall -I/c/cs323/Hwk4/
HWK = /c/cs323/Hwk4
OBJ = stack.o hashtable.o helper.o $(HWK)/code.o
CLEAN_OBJ = stack.o hashtable.o helper.o lzw.o test.o
#-----------------------------------------------------------
# Rules
all: encode decode test
encode: lzw.o $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
decode: encode
	ln -f encode decode
test: test.o $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
clean:
	rm -f encode decode test $(CLEAN_OBJ)
#-----------------------------------------------------------
# Dependencies
lzw.o: lzw.c hashtable.h stack.h helper.h $(HWK)/code.h
test.o: test.c hashtable.h stack.h helper.h $(HWK)/code.h
helper.o: helper.c helper.h
hashtable.o: hashtable.c hashtable.h helper.h $(HWK)/code.h stack.h
stack.o: stack.c stack.h
