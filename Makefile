# Makefile
#CC = gcc
CFLAGS += -g -pedantic -std=c99 -Wall -Wextra -Werror
LDFLAGS += -lm
LDLIBS += -lm

#SOURCES := $(wildcard *.c)
#OBJECTS := $(SOURCES:.c=.o)
#HEADERS := $(wildcard *.h)
TARGETS := 	tests

all: $(TARGETS)

valgrind: tests.o
	valgrind --leak-check=full --track-origins=yes ./tests

gdb: tests.o
	gdb ./tests

#$(TARGETS): $(OBJECTS)
#	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

graph.o: graph.h

tree.o: tree.h dijkstra.o graph.o

fibheaptest: fibheap.o

fibheap.o: fibheap.h

bheap.o: bheap.h

dijkstra.o: dijkstra.h fibheap.o bheap.o graph.o

thorup04.o: graph.o tree.o dijkstra.o

tests: tree.o dijkstra.o fibheap.o bheap.o graph.o thorup04.o

clean:
	@- rm -f *.o
	@- rm -f $(TARGETS)
	@- rm -f *.gch
	@- rm -f *~

.PHONY: all clean
