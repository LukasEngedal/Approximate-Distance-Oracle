#include <stdlib.h>

#include "dllist.h"

void dllist_init(dllist_t *dllist) {
    dllist->firstNode = NULL;
    dllist->lastNode = NULL;
}

void dlnode_init(dlnode_t *dlnode) {
    dlnode->prev = NULL;
    dlnode->next = NULL;
    dlnode->value = 0;
}

int dlnode_link(dlnode_t *dlnode1, dlnode_t *dlnode2) {
    dlnode1->next = dlnode2;
    dlnode2->prev = dlnode1;
    return 0;
}


