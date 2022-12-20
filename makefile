CC ?= gcc
CFLAGS += -Wall --pedantic -Werror
LIB_PATH = .

all: 
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o tas $(CFLAGS) tas.c main.c linked_list/linked_list.c logger.c

test:
	$(CC) $(LDFLAGS) -L$(LIB_PATH) -o test_tas $(CFLAGS) test.c tas.c linked_list/linked_list.c logger.c -lcunit -D TEST

clean:
	rm -f *.o tas test_tas