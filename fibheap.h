#ifndef FIBHEAP_H
#define FIBHEAP_H

typedef struct node_t {
    struct node_t* parent;
    struct node_t* child;
    struct node_t* next;
    struct node_t* prev;
    int value;
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

/* Initializes a doubly linked list node */
void node_init(node_t *node);

/* Initializes a Fibonacci heap */
void fibheap_init(fibheap_t *fibheap);

/* Adds a node to a (possibly empty) list */
void dllist_insert(dllist_t *dllist, node_t *node);

/* Removes a node from a list */
void dllist_remove(dllist_t *dllist, node_t *node);

/* Merges two lists */
dllist_t *dllist_merge(dllist_t *dllist1, dllist_t *dllist2);

/* Inserts a node into a Fibonacci heap */
void fibheap_insert(fibheap_t *fibheap, node_t *node);

/* Extracts the minimum node from a Fibonacci heap */
node_t *fibheap_extract_min(fibheap_t *fibheap);

/* Prints the values of all the nodes in a Fibonacci heap */
void fibheap_print(fibheap_t *fibheap);

#endif
