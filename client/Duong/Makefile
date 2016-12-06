
all:
	gcc -I ~/libs/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test.c -o test.o
	gcc test.o -Wall -lm -lev3dev-c -o test
run:
	./test
