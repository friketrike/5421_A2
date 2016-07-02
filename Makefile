# Makefile for assignennt 2, COMP 5421, summer 2016
# Federico O'Reilly Regueiro 40012304
# Concordia University
#
# This is based on the makefile tutorial that
# lives here http://mrbook.org/blog/tutorials/make/
#
CC=g++
#DX=doxygen
CFLAGS=-c -Wall -std=c++11
LDFLAGS=

DX := $(shell command -v doxygen 2> /dev/null)
LTX := $(shell command -v latex 2> /dev/null)

all: led docs
ifndef DX #TODO next time figure how to check for latex too 
	$(warning "doxygen is not available, skipping docs")
docs:
else #TODO define a latex rule here too...
docs: doxy
endif 

#all: led

led: led.o Command.o LineEditor.o
	$(CC) $(LDFLAGS) driver.o Command.o LineEditor.o -o $@

led.o: driver.cpp Command.cpp LineEditor.cpp
	$(CC) $(CFLAGS) driver.cpp Command.cpp LineEditor.cpp

doxy: Doxyfile Command.h LineEditor.h README.md
	$(DX) Doxyfile 

.PHONY: clean
clean:
	rm -rf *.o *~

