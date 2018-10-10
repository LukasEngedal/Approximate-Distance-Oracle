# Makefile
#CC = gcc
CFLAGS += -g -pedantic -std=gnu99 -Wall -Wextra -Werror
LDFLAGS += -lm -lrt
LDLIBS += -lm -lrt

#SOURCES := $(wildcard *.c)
#OBJECTS := $(SOURCES:.c=.o)
#HEADERS := $(wildcard *.h)
TARGETS := tests benchmark

all: $(TARGETS)

test: tests

valgrind: tests.o
	valgrind --leak-check=full --track-origins=yes ./tests

gdb: tests.o
	gdb ./tests

#$(TARGETS): $(OBJECTS)
#	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

graph.o: graph.h tree.c

tree.o: tree.h dijkstra.o graph.o queue.o

queue.o: queue.h

fibheaptest: fibheap.o

fibheap.o: fibheap.h

bheap.o: bheap.h

dijkstra.o: dijkstra.h fibheap.o bheap.o graph.o

thorup04.o: graph.o tree.o dijkstra.o

test_funcs.o: test_funcs.h graph.o tree.o

benchmark: graph.o thorup04.o tree.o dijkstra.o queue.o bheap.o fibheap.o test_funcs.o

tests: tree.o dijkstra.o fibheap.o bheap.o graph.o thorup04.o queue.o test_funcs.o

clean:
	@- rm -f *.o
	@- rm -f $(TARGETS)
	@- rm -f *.gch
	@- rm -f *~

.PHONY: all clean
