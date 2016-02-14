#include <stdio.h>
#include <stdlib.h>

#include "fibheap.h"

int main() {
    fibheap_t fibheap;
    fibheap_init(&fibheap);
    
    int n = 5;
    int A[5] = {5, 3, 7, 11, 9};
    node_t *node;
    for (int i = 0; i < n; i++) {
        node = malloc(sizeof(node_t));
        node_init(node);
        node->key = A[i];
        fibheap_insert(&fibheap, node);
    }
    
    printf("Fibheap List :\n");
    dllist_print(fibheap.rootList);
    //printf("min: %d\n", fibheap_extract_min(&fibheap)->key);
    return 0;
}
