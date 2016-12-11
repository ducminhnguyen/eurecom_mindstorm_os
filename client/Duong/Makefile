
CUR_PATH = $(shell pwd)

all:
	@echo $(CUR_PATH)
	gcc -I ~/libs/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test.c -o test.o
	gcc test.o -Wall -lm -lev3dev-c -L$(CUR_PATH)/../libs/bin -o test $(CUR_PATH)/../libs/bin/libmotorcontrol.o
run:
	./test
