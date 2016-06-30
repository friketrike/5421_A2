# Makefile for assignennt 2, COMP 5421, summer 2016
# Federico O'Reilly Regueiro 40012304
# Concordia University
#
# This is based on the makefile tutorial that
# lives here http://mrbook.org/blog/tutorials/make/
#
CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=

all: driver

driver: driver.o Command.o LineEditor.o
	$(CC) $(LDFLAGS) driver.o Command.o LineEditor.o -o $@

driver.o: driver.cpp Command.cpp LineEditor.cpp
	$(CC) $(CFLAGS) driver.cpp Command.cpp LineEditor.cpp

.PHONY: clean
clean:
	rm -f *.o *~ 

