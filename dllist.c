#include <stdlib.h>

#include "dllist.h"

void dllist_init(dllist_t *dllist) {
    dllist->firstNode = NULL;
}

void dlnode_init(dlnode_t *node) {
    node->parent = NULL;
    node->child = NULL;
    node->next = NULL;
    node->prev = NULL;
    node->value = 0;
    node->degree = 0;
    node->mark = 0;
}

void dllist_insert(dllist_t *dllist, dlnode_t *node) {
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

void dllist_remove(dllist_t *dllist, dlnode_t *node) {
    if (node->next == node) {
        dllist->firstNode = NULL;
    } else {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
}
