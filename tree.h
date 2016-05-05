#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "dijkstra.h"

typedef struct treenode_t {
    int id;
    int value;
    struct treenode_t *parent;
    struct treenode_t *child;
    struct treenode_t *sibling;
    void *owner;
} treenode_t;

typedef struct tree_t {
    int size;
    struct treenode_t *root;
} tree_t;

typedef struct lca_result_t {
    int length;
    int k;
    int *preorder;
    int *size;
    int *inlabel;
    int *ascendent;
    int *level;
    treenode_t **head;
} lca_result_t;

treenode_t *treenode_create(int value);

void treenode_destroy(treenode_t *treenode);

tree_t *tree_create(treenode_t *root);

void tree_destroy(tree_t *tree);

void tree_insert_treenode(tree_t *tree, treenode_t *parent, treenode_t *treenode);


tree_t *shortest_path_tree(graph_t *graph, int i);

path_t *tree_root_path(tree_t *tree, treenode_t *treenode);

int floor_log2(int x);
int ceil_log2(int x);

lca_result_t *lca_pre(tree_t *tree);

void lca_result_destroy(lca_result_t *result);

treenode_t *lca(lca_result_t *result, treenode_t *treenode1, treenode_t *treenode2);

#endif /*TREE_H */
