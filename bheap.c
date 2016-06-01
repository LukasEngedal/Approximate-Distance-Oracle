#include "bheap.h"
#include <math.h>

#define PARENT(i) (i - 1) >> 1
#define LEFT(i) (i << 1) + 1
#define RIGHT(i) (i << 1) + 2

bnode_t *bnode_create() {
    bnode_t *bnode = malloc(sizeof(bnode_t));
    if (bnode == NULL)
        return NULL;

    bnode->i = -1;
    bnode->key = 1<<30;
    bnode->owner = NULL;

    return bnode;
}

void bnode_destroy(bnode_t *bnode) {
    if (bnode != NULL)
        free(bnode);
}

bheap_t *bheap_create() {
    bheap_t *bheap = malloc(sizeof(bheap_t));
    if (bheap == NULL)
        return NULL;

    bheap->cap = BHEAP_INIT_SIZE;
    bheap->n = 0;

    bheap->A = malloc(sizeof(bnode_t *) * bheap->cap);
    if (bheap->A == NULL) {
        free(bheap);
        return NULL;
    }

    return bheap;
}

void bheap_destroy(bheap_t *bheap) {
    if (bheap != NULL) {
        for (int i = 0; i < bheap->n; i++) {
            bnode_destroy(bheap->A[i]);
        }
        free(bheap->A);
        free(bheap);
    }
}

void swap(bheap_t* bheap, bnode_t *bnode1, bnode_t *bnode2) {
    int temp = bnode1->i;
    bheap->A[bnode1->i] = bnode2;
    bheap->A[bnode2->i] = bnode1;
    bnode1->i = bnode2->i;
    bnode2->i = temp;
}

void min_heapify(bheap_t *bheap, int i) {
    int smallest;
    int l = LEFT(i);
    int r = RIGHT(i);
    if (l < bheap->n && bheap->A[l]->key < bheap->A[i]->key) {
        smallest = l;
    } else {
        smallest = i;
    }
    if (r < bheap->n && bheap->A[r]->key < bheap->A[smallest]->key) {
        smallest = r;
    }
    if (smallest != i) {
        swap(bheap, bheap->A[i], bheap->A[smallest]);
        min_heapify(bheap, smallest);
    }
}

int bheap_insert(bheap_t *bheap, bnode_t *bnode, int key) {
    if (bheap->n >= bheap->cap) {
        bheap->cap *= 2;
        bnode_t **new_A = realloc(bheap->A, sizeof(bnode_t *) * bheap->cap);
        if (new_A == NULL)
            return -1;
        bheap->A = new_A;
    }

    bnode->i = bheap->n;
    bheap->A[bheap->n] = bnode;
    bheap->n++;

    bheap_decrease_key(bheap, bnode, key);
    return 0;
}

bnode_t *bheap_extract_min(bheap_t *bheap) {
    if (bheap_empty(bheap))
        return NULL;

    bnode_t *bnode = bheap->A[0];
    if (bheap->n == 1) {
        bheap->A[0] = NULL;
        bheap->n--;
        return bnode;
    }

    bheap->A[0] = bheap->A[bheap->n - 1];
    bheap->A[bheap->n - 1] = NULL;
    bheap->A[0]->i = 0;
    bheap->n--;
    min_heapify(bheap, 0);
    return bnode;
}

int bheap_decrease_key(bheap_t *bheap, bnode_t *bnode, int key) {
    if (key < 0)
        printf("bheap_decrease_key: negative key: %d\n", key);
    if (key > bnode->key)
        return -1;

    bnode->key = key;
    while (bnode->i > 0 && bheap->A[PARENT(bnode->i)]->key > bnode->key) {
        swap(bheap, bnode, bheap->A[PARENT(bnode->i)]);
    }
    return 0;
}
