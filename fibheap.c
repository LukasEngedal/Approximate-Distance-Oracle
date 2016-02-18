#include "fibheap.h"

void node_init(node_t *node) {
    node->parent = NULL;
    node->child = NULL;
    node->next = NULL;
    node->prev = NULL;
    node->key = 0;
    node->degree = 0;
    node->mark = 0;
}

void fibheap_init(fibheap_t *fibheap) {
    fibheap->min = NULL;
    fibheap->n = 0;
}

void dllist_insert(node_t *node, node_t *newNode) {
    if (node == NULL || newNode == NULL) {
        printf("Error: Node is not initialized!\n");
        exit(EXIT_FAILURE);
    }
    if (node->next == NULL) {
        printf("Error: Node does not belong to any list!\n");
        exit(EXIT_FAILURE);
    }
    newNode->next = node->next;
    node->next->prev = newNode;
    newNode->prev = node;
    node->next = newNode;
}

void dllist_remove(node_t *node) {
    if (node == NULL) {
        printf("Error: Node is not initialized!\n");
        exit(EXIT_FAILURE);
    }
    if (node->next == NULL) {
        printf("Error: Node does not belong to any list!\n");
        exit(EXIT_FAILURE);
    }
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

void dllist_merge(node_t *node1, node_t *node2) {
    if (node1 == NULL || node2 == NULL) {
        printf("Error: Node is not initialized!\n");
        exit(EXIT_FAILURE);
    }
    if (node1->next == NULL || node2->next == NULL) {
        printf("Error: Node does not belong to any list!\n");
        exit(EXIT_FAILURE);
    }
    node_t *prev1 = node1->prev;
    node_t *prev2 = node2->prev;
    prev1->next = node2;
    prev2->next = node1;
    node1->prev = prev2;
    node2->prev = prev1;
}

void dllist_create(node_t *node) {
    if (node == NULL) {
        printf("Error: Node is not initialized!\n");
        exit(EXIT_FAILURE);
    }
    node->next = node;
    node->prev = node;
}

void fibheap_insert(fibheap_t *fibheap, node_t *node) {
    if (node == NULL) {
        printf("Error: Node is not initialized!\n");
        exit(EXIT_FAILURE);
    }
    if (fibheap->min == NULL) {
        dllist_create(node);
        fibheap->min = node;
    } else {
        dllist_insert(fibheap->min, node);
        if (node->key < fibheap->min->key) {
            fibheap->min = node;
        }
    }
    fibheap->n++;
}

void fibheap_consolidate(fibheap_t *fibheap) {
    printf("Consolidate\n");
    int n = ceil(log(fibheap->n)) + 1;
    printf("D(n) = %d\n", n - 1);
    node_t *A[n];
    for (int i = 0; i < n; i++) {
        A[i] = NULL;
    }
    
    node_t *x = fibheap->min;
    node_t *next = x->next;
    node_t *end = x->prev;
    node_t *y;
    node_t *temp;
    do {
        printf("Consolidate Do\n");
        printf("Node %d\n", x->key);
        printf("Next %d\n", x->next->key);
        printf("Start %d\n", start->key);
        while (A[x->degree] != NULL) {
            if (x->degree >= n) {
                printf("Error: Node degree %d exceeds D(n) = %d!\n", x->degree, n - 1);
                exit(EXIT_FAILURE);
            }
            printf("Consolidate While\n");
            printf("x.degree = %d\n", x->degree);
            y = A[x->degree];
            if (x->key > y->key) {
                temp = x;
                x = y;
                y = temp;
            }
            dllist_remove(y);
            if (x->child == NULL) {
                dllist_create(y);
                x->child = y;
            } else {
                dllist_insert(x->child, y);
            }
            y->mark = 0;
            A[x->degree] = NULL;
            x->degree++;
        }
        A[x->degree] = x;
        x = next;
        next = x->next;
    } while (x != end);
    fibheap->min = NULL;

    for (int i = 0; i < n; i++) {
        if (A[i] != NULL) {
            if (fibheap->min == NULL) {
                dllist_create(A[i]);
                fibheap->min = A[i];
            } else {
                dllist_insert(fibheap->min, A[i]);
                if (fibheap->min->key > A[i]->key) {
                    fibheap->min = A[i];
                }
            }
        }
    }
}

node_t *fibheap_extract_min(fibheap_t *fibheap) {
    printf("Extract min\n");
    node_t *z = fibheap->min;
    if (z != NULL) {
        if (z->child != NULL) {
            node_t *current = z->child;
            for (int i = 0; i < z->mark; i++) {
                current->parent = NULL;
                current = current->next;
            }
            dllist_merge(z, z->child);
        }
        dllist_remove(z);
        if (z == z->next) {
            fibheap->min = NULL;
        } else {
            fibheap->min = z->next;
            fibheap_consolidate(fibheap);
        }
        fibheap->n--;
    }
    return z;
}

void fibheap_cut(fibheap_t *fibheap, node_t *node, node_t *parent) {
    if (node->next == node) {
        parent->child = NULL;
    } else {
        dllist_remove(node);
        parent->child = node->next;
    }
    parent->degree--;
    dllist_insert(fibheap->min, node);
    node->parent = NULL;
    node->mark = 0;
}

void fibheap_cascadingCut(fibheap_t *fibheap, node_t *node) {
    node_t *parent = node->parent;
    if (parent != NULL) {
        if (node->mark == 0) {
            node->mark = 1;
        } else {
            fibheap_cut(fibheap, node, parent);
            fibheap_cascadingCut(fibheap, parent);
        }
    }
}

void fibheap_decreaseKey(fibheap_t *fibheap, node_t *node, int k) {
    if (k > node->key) {
        printf("Error: New key is greater than current key!\n");
        exit(EXIT_FAILURE);
    }
    node->key = k;
    node_t *parent = node->parent;
    if (parent != NULL && node->key < parent->key) {
        fibheap_cut(fibheap, node, parent);
        fibheap_cascadingCut(fibheap, parent);
    }
    if (node->key < fibheap->min->key) {
        fibheap->min = node;
    }
}

void tree_print(node_t *node, int d) {
    for (int i = 0; i < d; i++) {
        printf("  ");
    }
    //printf("%2d <- %2d -> %d\n", node->prev->key, node->key, node->next->key);
    printf("%d\n", node->key);

    node_t *child = node->child;
    if (child != NULL) {
        do {
            tree_print(child, d + 1);
            child = child->next;
        } while (child != node->child);
    }
}

void fibheap_print(fibheap_t *fibheap) {
    printf("Fibheap:\n");

    node_t *current = fibheap->min;
    if (current != NULL) {
        do {
            tree_print(current, 0);
            current = current->next;
        } while (current != fibheap->min);
    } else {
        printf("NULL\n");
    }
}
