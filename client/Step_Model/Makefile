
CUR_PATH = $(shell pwd)
LIB_PATH = $(CUR_PATH)/../libs/bin/
ALL_LIB = $(filter-out ../libs/bin/bluetooth.o, $(patsubst ../libs/movement/%.c, ../libs/bin/%.o, $(wildcard ../libs/movement/*.c)))


CFLAGS 		= -Werror -pedantic -std=gnu99 -g
LFLAGS 		= -lbluetooth -lcurses -lpthread

all: $(ALL_LIB)
	@echo $(ALL_LIB)
	# cd ../libs && $(MAKE)
	bash ./allheader
	gcc -I ~/libs/ev3dev-c/source/ev3 -O2 -std=gnu99 -c main.c -o main.o
	gcc main.o -Wall -lm -lev3dev-c -L$(LIB_PATH) -o test $^

../libs/bin/%.o: ../libs/movement/%.c
	gcc $(CFLAGS) -c -o $@ $< $(LFLAGS)

run:
	./test
