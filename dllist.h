#ifndef DLLIST_H
#define DLLIST_H

typedef struct dlnode_t {
  struct dlnode_t* prev;
  struct dlnode_t* next;
  int value;
} dlnode_t;

typedef struct dllist_t {
  struct dlnode_t* firstNode;
  struct dlnode_t* lastNode;
} dllist_t;

/* Initializes a doubly linked list */
void dllist_init(dllist_t *dllist);

/* Initializes a doubly linked list node */
void dlnode_init(dlnode_t *node);

/* Adds a node to a list */
int dlnode_insertAfter(dlnode_t *node, dlnode_t *newNode);

#endif
