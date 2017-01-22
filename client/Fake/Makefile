CC 		= gcc
CFLAGS 		= -Werror -pedantic -std=gnu89 -g
LFLAGS 		= -lbluetooth -lcurses -lpthread
SRC_FILES 	= $(wildcard *.c)
OBJ_FILES	= $(SRC_FILES:%.c=%.o)
EXEC		= client

all: $(EXEC)

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

$(EXEC): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LFLAGS)

.PHONY: clean ultraclean

clean:
	rm -f $(OBJ_FILES)

ultraclean: clean
	rm -f $(EXEC)
