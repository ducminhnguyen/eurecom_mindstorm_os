#! /bin/bash
# This is the make file for the libraries in this dir
# any changes to the code in this library must be recompile with this file before including the library to your project

# compile library

CC 		= gcc
CFLAGS 		= -Werror -pedantic -std=gnu99 -g
LFLAGS 		= -lbluetooth -lcurses -lpthread

SRC=$(wildcard movement/*.c)

PROG=$(patsubst movement/%.c,bin/%.o, $(SRC))


all: $(PROG)

bin/%.o: movement/%.c
	mkdir -p bin
	$(CC) $(CFLAGS) -c -o $@ $< $(LFLAGS)