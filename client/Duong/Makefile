
CUR_PATH = $(shell pwd)
LIB_PATH = $(CUR_PATH)/../libs/bin/
all:
	@echo $(CUR_PATH)
	gcc -I ~/libs/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test.c -o test.o
	gcc test.o -Wall -lm -lev3dev-c -L$(LIB_PATH) -o test $(LIB_PATH)libmotorcontrol.o
run:
	./test
