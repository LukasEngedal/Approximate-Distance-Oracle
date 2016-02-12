#include <stdlib.h>

#include "dllist.h"

void dllist_init(dllist_t *dllist) {
  dllist->firstNode = NULL;
  dllist->lastNode = NULL;
}

void dlnode_init(dlnode_t *node) {
  dlnode->prev = NULL;
  dlnode->next = NULL;
  dlnode->value = 0;
}

int dlnode_insertAfter(dlnode_t *node, dlnode_t *newNode) {
  newNode->next = node->next;
  node->next = newNode;
  newNode->prev = node;
  return 0;
}


