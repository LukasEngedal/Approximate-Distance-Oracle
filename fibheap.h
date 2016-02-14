#ifndef FIBHEAP_H
#define FIBHEAP_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct node_t {
    struct node_t* parent;
    struct dllist_t* children;
    struct node_t* next;
    struct node_t* prev;
    int key;
    int degree;
    int mark;
} node_t;

typedef struct dllist_t {
    struct node_t* firstNode;
} dllist_t;

typedef struct fibheap_t {
    struct dllist_t* rootList;
    struct node_t* min;
    int n;
} fibheap_t;

/* Initializes a doubly linked list */
void dllist_init(dllist_t *dllist);

/* Initializes a node */
void node_init(node_t *node);

/* Initializes a Fibonacci heap */
void fibheap_init(fibheap_t *fibheap);

/* Adds a node to a list */
void dllist_insert(dllist_t *dllist, node_t *node);

/* Removes a node from a list */
void dllist_remove(dllist_t *dllist, node_t *node);

/* Merges two lists, saving the result as the first list and freeing the second */
void dllist_merge(dllist_t *dllist1, dllist_t *dllist2);

/* Inserts a node into a Fibonacci heap */
void fibheap_insert(fibheap_t *fibheap, node_t *node);

/* Extracts the minimum node from a Fibonacci heap */
node_t *fibheap_extract_min(fibheap_t *fibheap);

/* Consolidates the root list of a Fibonacci heap */
void fibheap_consolidate(fibheap_t *fibheap);

/* Prints the keys of all the nodes in a list */
void dllist_print(dllist_t *dllist);

#endif
