CC=gcc
CFLAGS=-c -Wall -O3

all: lib doc

doc: Doxyfile $(wildcard *.dox) $(wildcard *.h)
	doxygen

# Static library
lib: linkedList.o stack.o
	ar rcs libCS101.a linkedList.o stack.o

linkedList.o: linkedList.c
	$(CC) $(CFLAGS) linkedList.c

stack.o: stack.c linkedList.o
	$(CC) $(CFLAGS) stack.c

clean:
	rm -rf *o
