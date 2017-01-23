
CUR_PATH = $(shell pwd)
LIB_PATH = $(CUR_PATH)/../libs/bin/
ALL_LIB = $(patsubst ../libs/movement/%.c, ../libs/bin/%.o, $(wildcard ../libs/movement/*.c))


CFLAGS 		= -Werror -pedantic -std=gnu99 -g
LFLAGS 		= -lbluetooth -lcurses -lpthread

all: $(ALL_LIB)
	@echo $(ALL_LIB)
	# cd ../libs && $(MAKE)
	bash ./allheader
	gcc -lm -I ~/libs/ev3dev-c/source/ev3 -O2 -std=gnu99 -c main.c -o main.o
	gcc main.o -lm -Wall -lev3dev-c $(LFLAGS) -L$(LIB_PATH) -o test $^

../libs/bin/%.o: ../libs/movement/%.c
	gcc $(CFLAGS) -lm -c -o $@ $< $(LFLAGS)

run:
	./test
