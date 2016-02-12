CC=gcc
CFLAGS=-g -pedantic -std=c99 -Wall -Wextra -Werror

all:
	make dllist

dllist: dllist.c dllist.h
	$(CC) $(CFLAGS) -c dllist.c dllist.h

clean:
	rm -f *.o
