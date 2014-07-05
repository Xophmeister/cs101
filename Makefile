CC=gcc
CFLAGS=-fpic -O3
VPATH=indexed:graph

all: static shared doc

clean:
	rm -rf *.o

.PHONY: all clean static shared

# Source
objects=dynamicArray.o directedGraph.o linkedList.o stack.o

dynamicArray.o: dynamicArray.c dynamicArray.h
directedGraph.o: directedGraph.c directedGraph.h dynamicArray.h
linkedList.o: linkedList.c directedGraph.h linkedList.h 
stack.o: stack.c linkedList.h stack.h 

# Static library
static: libCS101.a

libCS101.a: $(objects)
	$(AR) rcs $@ $^

# Shared library
shared: libCS101.so

libCS101.so: $(objects)
	$(CC) -shared -o $@ $^

# Documentation
doc: Doxyfile $(shell find . -name "*.dox" -or -name "*.h")
	doxygen
