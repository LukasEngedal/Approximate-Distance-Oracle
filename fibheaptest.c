#include <stdio.h>
#include <stdlib.h>

#include "fibheap.h"

int main() {
    fibheap_t fibheap;
    fibheap_init(&fibheap);

    int n = 5;
    node_t *node;
    for (int i = 0; i < n; i++) {
        node = malloc(sizeof(node_t));
        node_init(node);
        node->value = i;
        fibheap_insert(&fibheap, node);
    }

    fibheap_print(&fibheap);
    printf("min: %d\n", fibheap_extract_min(&fibheap)->value);
    return 0;
}
