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
    
    fibheap_print(&fibheap);
    printf("Decrease key 9 to 1.\n");
    fibheap_decrease_key(&fibheap, node, 1);
    fibheap_print(&fibheap);

    //fibheap_destroy(&fibheap);

    for (int i = 0; i < n; i++) {
        node_t *min = fibheap_extract_min(&fibheap);
        if (min != NULL) {
            printf("Extract min: %d\n", min->key);
        }
        fibheap_print(&fibheap);
    }
    return 0;
}
