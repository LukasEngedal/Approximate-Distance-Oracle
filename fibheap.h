#ifndef FIBHEAP_H
#define FIBHEAP_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct node_t {
    struct node_t* parent;
    struct node_t* child;
    struct node_t* next;
    struct node_t* prev;
    int key;
    int degree;
    int mark;
} node_t;

typedef struct fibheap_t {
    struct node_t* min;
    int n;
} fibheap_t;

/* Initializes a node */
void node_init(node_t *node);

/* Initializes a Fibonacci heap */
void fibheap_init(fibheap_t *fibheap);

/* Inserts a node into a Fibonacci heap */
void fibheap_insert(fibheap_t *fibheap, node_t *node);

/* Extracts the minimum node from a Fibonacci heap */
node_t *fibheap_extract_min(fibheap_t *fibheap);

/* Decreases the key of a node in a Fibonacci heap */
void fibheap_decreaseKey(fibheap_t *fibheap, node_t *node, int k);

/* Prints the keys of all the nodes in a list */
void fibheap_print(fibheap_t *fibheap);

#endif
