#! /bin/bash
# This is the make file for the libraries in this dir
# any changes to the code in this library must be recompile with this file before including the library to your project

# compile library

CC 		= gcc
CFLAGS 		= -Werror -pedantic -std=gnu99 -g
LFLAGS 		= -lbluetooth -lcurses -lpthread

all:
	$(CC) $(CFLAGS) -I./ev3dev-c/source/ev3 -o bin/libmotorcontrol.o -c movement/motorControl.c