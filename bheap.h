#ifndef BHEAP_H
#define BHEAP_H

#include <stdlib.h>
#include <stdio.h>

#define BHEAP_INIT_SIZE 8

typedef struct bnode_t {
    int i;
    int key;
    void *owner;
} bnode_t;

typedef struct bheap_t {
    int cap;
    int n;
    bnode_t **A;
} bheap_t;

bnode_t *bnode_create();

bheap_t *bheap_create();

void bnode_destroy(bnode_t *bnode);

void bheap_destroy(bheap_t *bheap);

int bheap_insert(bheap_t *bheap, bnode_t *bnode, int key);

bnode_t *bheap_extract_min(bheap_t *bheap);

int bheap_decrease_key(bheap_t *bheap, bnode_t *bnode, int key);

#define bheap_empty(bheap) (!bheap->n)

#endif /* BHEAP_H */
