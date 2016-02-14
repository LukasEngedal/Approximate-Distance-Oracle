CC=gcc
CFLAGS=-g -pedantic -std=c99 -Wall -Wextra -Werror

fibheap.o: fibheap.c fibheap.h
	$(CC) $(CFLAGS) -c fibheap.c fibheap.h

fibheaptest.o: fibheap.h fibheaptest.c
	$(CC) $(CFLAGS) -c fibheaptest.c

fibheaptest: fibheaptest.o fibheap.o
	$(CC) $(CFLAGS) fibheaptest.o fibheap.o -o fibheaptest -lm

clean:
	rm -f *.o
	rm -f *.gch
	rm -f fibheaptest

all:
	make fibheap.o --no-print-directory
	make fibheaptest.o --no-print-directory
	make fibheaptest --no-print-directory
