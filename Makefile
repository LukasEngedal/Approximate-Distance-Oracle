CC=gcc
CFLAGS=-g -pedantic -std=c99 -Wall -Wextra -Werror

fibheap.o: fibheap.c fibheap.h
	$(CC) $(CFLAGS) -c fibheap.c fibheap.h

fibheaptest.o: fibheap.h fibheaptest.c
	$(CC) $(CFLAGS) -c fibheaptest.c

fibheaptest: fibheaptest.o fibheap.o
	$(CC) $(CFLAGS) fibheaptest.o fibheap.o -o fibheaptest -lm

graph.o: graph.c graph.h
	$(CC) $(CFLAGS) -c graph.c graph.h

graphtest.o: graph.h graphtest.c
	$(CC) $(CFLAGS) -c graphtest.c

graphtest: graphtest.o graph.o fibheap.o
	$(CC) $(CFLAGS) graphtest.o graph.o fibheap.o -o graphtest -lm

dijkstra.o: dijkstra.c dijkstra.h
	$(CC) $(CFLAGS) -c dijkstra.c

dijkstratest.o: dijkstratest.c dijkstra.h
	$(CC) $(CFLAGS) -c dijkstratest.c

dijkstratest: dijkstratest.o dijkstra.o graph.o fibheap.o
	$(CC) $(CFLAGS) dijkstratest.o dijkstra.o graph.o fibheap.o -o dijkstratest -lm

clean:
	rm -f *.o
	rm -f *.gch
	rm -f fibheaptest
	rm -f graphtest
	rm -f dijkstratest

all:
	make fibheap.o --no-print-directory
	make fibheaptest.o --no-print-directory
	make fibheaptest --no-print-directory
	make graph.o --no-print-directory
	make graphtest.o --no-print-directory
	make graphtest --no-print-directory
	make dijkstra.o --no-print-directory
	make dijkstratest.o --no-print-directory
	make dijkstratest --no-print-directory
