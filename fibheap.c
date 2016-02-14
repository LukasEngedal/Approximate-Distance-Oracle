#include "fibheap.h"

void dllist_init(dllist_t *dllist) {
    dllist->firstNode = NULL;
}

void node_init(node_t *node) {
    node->parent = NULL;
    node->children = NULL;
    node->next = NULL;
    node->prev = NULL;
    node->key = 0;
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
    }
}

void dllist_merge(dllist_t *dllist1, dllist_t *dllist2) {
    if (dllist2 != NULL) {
        if (dllist2->firstNode != NULL) {
            node_t *node1 = dllist1->firstNode;
            node_t *node2 = dllist2->firstNode;
            node_t *next1 = node1->next;
            node_t *next2 = node2->next;
            node1->next = next2;
            node2->next = next1;
            next1->prev = node2;
            next2->prev = node1;
            free(dllist2);
        }
    }
}

void fibheap_insert(fibheap_t *fibheap, node_t *node) {
    if (fibheap->min == NULL) {
        dllist_t *dllist = malloc(sizeof(dllist_t));
        dllist_init(dllist);
        dllist_insert(dllist, node);
        fibheap->rootList = dllist;
        fibheap->min = node;
    } else {
        dllist_insert(fibheap->rootList, node);
        if (node->key < fibheap->min->key) {
            fibheap->min = node;
        }
    }
    fibheap->n++;
}

node_t *fibheap_extract_min(fibheap_t *fibheap) {
    node_t *z = fibheap->min;
    if (z != NULL) {
        /* z' childrens p-attribute is not set to NULL */
        dllist_merge(fibheap->rootList, z->children);
        dllist_remove(fibheap->rootList, z);
        if (z->next == z) {
            fibheap->min = NULL;
        } else {
            fibheap->min = z->next;
            fibheap_consolidate(fibheap);
        }
    }
    fibheap->n--;
    return z;
}

void fibheap_consolidate(fibheap_t *fibheap) {
    int n = ceil(log(fibheap->n));
    node_t *A[n];
    for (int i = 0; i < n; i++) {
        A[i] = NULL;
    }
    
    node_t *x = fibheap->min;
    node_t *next = x->next;
    node_t *y;
    node_t *temp;
    for (int i = 0; i < fibheap->n; i++) {
        while (A[x->degree] != NULL) {
            y = A[x->degree];
            if (x->key > y->key) {
                temp = x;
                x = y;
                y = temp;
            }
            dllist_remove(fibheap->rootList, y);
            dllist_insert(x->children, y);
            A[x->degree] = NULL;
            x->degree++;
            y->mark = 0;
        }
        A[x->degree] = x;
        x = next;
        next = x->next;
    }
    fibheap->min = NULL;
    fibheap->rootList->firstNode = NULL;
    for (int i = 0; i < n; i++) {
        if (A[i] != NULL) {
            dllist_insert(fibheap->rootList, A[i]);
            if (fibheap->min->key > A[i]->key) {
                fibheap->min = A[i];
            }
        }
    }
}

void dllist_print(dllist_t *dllist) {
    node_t* current = dllist->firstNode;
    do {
        printf("%d\n", current->key);
        current = current->next;
    } while (current != dllist->firstNode);
}
