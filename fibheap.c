#include <stdlib.h>
#include <stdio.h>

#include "fibheap.h"

void dllist_init(dllist_t *dllist) {
    dllist->firstNode = NULL;
}

void node_init(node_t *node) {
    node->parent = NULL;
    node->child = NULL;
    node->next = NULL;
    node->prev = NULL;
    node->value = 0;
    node->degree = 0;
    node->mark = 0;
}

void fibheap_init(fibheap_t *fibheap) {
    fibheap->rootList = NULL;
    fibheap->min = NULL;
    fibheap->n = 0;
}

void dllist_insert(dllist_t *dllist, node_t *node) {
    if (dllist->firstNode == NULL) {
        dllist->firstNode = node;
        node->next = node;
        node->prev = node;
    } else {
        node->prev = dllist->firstNode->prev;
        node->next = dllist->firstNode;
        dllist->firstNode->prev->next = node;
        dllist->firstNode->prev = node;
    }
}

void dllist_remove(dllist_t *dllist, node_t *node) {
    if (node->next == NULL) {
        printf("Error: Node does not belong to any list!\n");
        exit(EXIT_FAILURE);
    }
    if (node->next == node) {
        dllist->firstNode = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        dllist->firstNode = node->next;
        node->next = NULL;
        node->prev = NULL;
    }
}

dllist_t *dllist_merge(dllist_t *dllist1, dllist_t *dllist2) {
    if (dllist1->firstNode == NULL) {return dllist2;}
    if (dllist2->firstNode == NULL) {return dllist1;}
    node_t *node1 = dllist1->firstNode;
    node_t *node2 = dllist2->firstNode;
    node_t *next1 = node1->next;
    node_t *next2 = node2->next;
    node1->next = next2;
    node2->next = next1;
    next1->prev = node2;
    next2->prev = node1;
    return dllist1;
}

void fibheap_insert(fibheap_t *fibheap, node_t *node) {
    if (fibheap->min == NULL) {
        dllist_t dllist;
        dllist_init(&dllist);
        dllist_insert(&dllist, node);
        fibheap->rootList = &dllist;
        fibheap->min = node;
    } else {
        dllist_insert(fibheap->rootList, node);
        if (node->value < fibheap->min->value) {
            fibheap->min = node;
        }
    }
    fibheap->n++;
}

node_t *fibheap_extract_min(fibheap_t *fibheap) {
    
    return fibheap->min;
}

void fibheap_print(fibheap_t *fibheap) {
    node_t* current = fibheap->rootList->firstNode;
    int i = 0;
    while (i < fibheap->n) {
        printf("%d\n", current->value);
        current = current->next;
        i++;
    }
}
