#ifndef DLLIST_H
#define DLLIST_H

typedef struct dlnode_t {
    struct dlnode_t* parent;
    struct dlnode_t* child;
    struct dlnode_t* next;
    struct dlnode_t* prev;
    int value;
    int degree;
    int mark;
} dlnode_t;

typedef struct dllist_t {
    struct dlnode_t* firstNode;
} dllist_t;

typedef struct fibb_t {
    struct dllist_t* rootList;
    struct dlnode_t* min;
    int n;
} fibb_t;

/* Initializes a doubly linked list */
void dllist_init(dllist_t *dllist);

/* Initializes a doubly linked list node */
void dlnode_init(dlnode_t *node);

/* Adds a node to a (possibly empty) list */
void dlnode_insert(dllist_t *dllist, dlnode_t *node);

/* Removes a node from a list */
void dllist_remove(dllist_t *dllist, dlnode_t *node);

#endif
