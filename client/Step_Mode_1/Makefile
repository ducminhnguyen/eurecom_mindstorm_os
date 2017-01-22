
CUR_PATH = $(shell pwd)
LIB_PATH = $(CUR_PATH)/../libs/bin/
ALL_LIB = $(patsubst ../libs/movement/%.c, ../libs/bin/%.o, $(wildcard ../libs/movement/*.c))
ALL_HEADERS = $(wildcard ../libs/header/*.c)

CFLAGS 		= -Werror -pedantic -std=gnu99 -g
LFLAGS 		= -lbluetooth -lcurses -lpthread

all: $(ALL_LIB) $(ALL_HEADERS)
	@echo $(CUR_PATH)
	cd ../libs && $(MAKE)
	bash ./allheader
	gcc -I ~/libs/ev3dev-c/source/ev3 -O2 -std=gnu99 -c main.c -o main.o
	gcc main.o -Wall -lm -lev3dev-c -L$(LIB_PATH) -o test $^

../libs/bin/%.o: ../libs/movement/%.c
	gcc $(CFLAGS) -lm -c -o $@ $< $(LFLAGS)

run:
	./test
