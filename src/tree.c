#include <math.h>
#include "tree.h"
#include "queue.h"
#include "graph.h"

treenode_t *treenode_create(int value) {
    treenode_t *treenode = malloc(sizeof(treenode_t));
    if (treenode == NULL)
        return NULL;

    treenode->value = value;
    treenode->lvl = -1;
    treenode->size = -1;
    treenode->parent = NULL;
    treenode->child = NULL;
    treenode->sibling = NULL;
    treenode->owner = NULL;

    return treenode;
}

void treenode_destroy(treenode_t *treenode) {
    if (treenode != NULL)
        free(treenode);
}

tree_t *tree_create(treenode_t *root) {
    tree_t *tree = malloc(sizeof(tree_t));
    if (tree == NULL)
        return NULL;

    tree->root = root;
    tree->size = 1;
    root->id = 0;
    root->lvl = 0;

    return tree;
}

void tree_destroy_node(treenode_t *treenode) {
    if (treenode != NULL) {
        tree_destroy_node(treenode->sibling);
        tree_destroy_node(treenode->child);
        treenode_destroy(treenode);
    }
}

void tree_destroy(tree_t *tree) {
    if (tree != NULL) {
        tree_destroy_node(tree->root);
        free(tree);
    }
}

void tree_insert_treenode(tree_t *tree, treenode_t *parent, treenode_t *treenode) {
    if (parent == NULL || treenode == NULL) {
        printf("tree_insert_treenode: treenode or parent null.\n");
        exit(EXIT_FAILURE);
    }
    if (parent->child == NULL) {
        parent->child = treenode;
    } else {
        treenode_t *sibling = parent->child;
        while (sibling->sibling != NULL) {
            sibling = sibling->sibling;
        }
        sibling->sibling = treenode;
    }
    treenode->parent = parent;
    treenode->id = tree->size;
    tree->size++;
    treenode->lvl = parent->lvl + 1;
}

path_t *tree_root_path(treenode_t *treenode) {
    int lvl = treenode->lvl;
    path_t *path = path_create(lvl + 1);

    treenode_t *node = treenode;
    for (int i = lvl; i >= 0; i--) {
        path->V[i] = (vertex_t *)node->owner;
        path->d[i] = node->value;
        node = node->parent;
    }

    return path;
}

void tree_level_helper(treenode_t *treenode, int lvl) {
    if (treenode != NULL) {
        treenode->lvl = lvl;
        tree_level_helper(treenode->sibling, lvl);
        tree_level_helper(treenode->child, lvl + 1);
    }
}

void tree_set_levels(tree_t *tree) {
    tree_level_helper(tree->root, 0);
}

int tree_size_helper(treenode_t *treenode) {
    if (treenode == NULL)
        return 0;

    int siblings = tree_size_helper(treenode->sibling);
    int children = tree_size_helper(treenode->child);
    treenode->size = children + 1;
    return children + siblings + 1;
}

void tree_set_sizes(tree_t *tree) {
    tree_size_helper(tree->root);
}

tree_t *breath_first_tree(graph_t *graph, int id) {
    graph_reset_nodes(graph);

    vertex_t *vertex = graph->V[id];
    treenode_t *root = treenode_create(0);
    root->owner = (void *)vertex;
    vertex->node = (void *)root;
    tree_t *tree = tree_create(root);

    queue_t *Q = queue_create();
    queue_push(Q, (void *)vertex);

    vertex_t *v, *u;
    edge_t *e;
    treenode_t *vnode, *unode;
    while (Q->size > 0) {
        v = (vertex_t *)queue_pop(Q);
        vnode = (treenode_t *)v->node;
        e = v->edge;
        for (int i = 0; i < v->n; i++) {
            u = e->target;
            if (u->node == NULL) {
                unode = treenode_create(vnode->value + e->w);
                unode->owner = (void *)u;
                u->node = (void *)unode;
                tree_insert_treenode(tree, vnode, unode);
                queue_push(Q, (void *)u);
            }

            e = e->c;
        }
    }
    queue_destroy(Q);

    tree_set_sizes(tree);
    tree_set_levels(tree);
    return tree;
}

tree_t *shortest_path_tree(graph_t *graph, int s) {
    if (graph->V[s] == NULL)
        return NULL;

    dijkstra_result_t *result = dijkstra_sssp(graph, s);

    treenode_t *treenode;
    for (int i = 0; i < graph->cap_v; i++) {
        if (graph->V[i] == NULL)
            continue;

        treenode = treenode_create(0);
        treenode->owner = (void *)graph->V[i];
        graph->V[i]->node = (void *)treenode;
    }
    tree_t *tree = tree_create((treenode_t *)graph->V[s]->node);

    vertex_t *vertex;
    for (int i = 0; i < graph->cap_v; i++) {
        if (i == s)
            continue;

        vertex = graph->V[i];
        if (vertex == NULL)
            continue;

        if (result->prev[i] == NULL) {
            printf("Error (shortest_path_tree): Graph is not connected!\n");
            exit(EXIT_FAILURE);
        }

        treenode = (treenode_t *)vertex->node;
        treenode->value = result->dist[i];
        tree_insert_treenode(tree, (treenode_t *)result->prev[i]->node, treenode);
    }

    dijkstra_destroy(result);

    tree_set_sizes(tree);
    tree_set_levels(tree);
    return tree;
}


lca_result_t *lca_result_create(int n) {
    lca_result_t *result = malloc(sizeof(lca_result_t));
    if (result == NULL)
        return NULL;

    int *preorder = malloc(sizeof(int) * n);
    if (preorder == NULL) {
        free(result);
        return NULL;
    }

    int *size = malloc(sizeof(int) * n);
    if (size == NULL) {
        free(result);
        free(preorder);
        return NULL;
    }

    int *inlabel = malloc(sizeof(int) * n);
    if (inlabel == NULL) {
        free(result);
        free(preorder);
        free(size);
        return NULL;
    }

    int *ascendent = malloc(sizeof(int) * n);
    if (ascendent == NULL) {
        free(result);
        free(preorder);
        free(size);
        free(inlabel);
        return NULL;
    }

    int *level = malloc(sizeof(int) * n);
    if (level == NULL) {
        free(result);
        free(preorder);
        free(size);
        free(inlabel);
        free(ascendent);
        return NULL;
    }

    result->preorder = preorder;
    result->size = size;
    result->inlabel = inlabel;
    result->ascendent = ascendent;
    result->level = level;
    result->length = 0;
    result->k = 0;

    return result;
}

int first_transversal(treenode_t *treenode, lca_result_t *result) {
    if (treenode == NULL)
        return 0;

    int id = treenode->id;

    /* Set preorder as the number of nodes visited so far */
    int preorder = 1 + result->length++;
    result->preorder[id] = preorder;

    /* Determine the size of the subtrees rooted at 'child', and its siblings,
     * and ay 'sibling', and its siblings */
    int child = first_transversal(treenode->child, result);
    int siblings = first_transversal(treenode->sibling, result);
    int size = child + 1;
    result->size[id] = size;

    /* Determine the inlabel */
    int i = floor_log2((preorder - 1) ^ (preorder + size - 1));
    int inlabel = (1 << i) * ((preorder + size - 1) >> i);
    if (inlabel > result->k)
        result->k=inlabel;
    result->inlabel[id] = inlabel;

    return size + siblings;
}

void second_transversal(treenode_t *treenode, lca_result_t *result) {
    if (treenode == NULL)
        return;

    int id = treenode->id;
    int pid = treenode->parent->id;

    /* Set the level of node */
    result->level[id] = result->level[pid] + 1;

    /* Determine ascendent of the node, depending on the parent */
    if (result->inlabel[id] == result->inlabel[pid]) {
        result->ascendent[id] = result->ascendent[pid];
    } else {
        int j = result->inlabel[id] - (result->inlabel[id] & (result->inlabel[id] - 1));
        int i = floor_log2(j);
        result->ascendent[id] = result->ascendent[pid] + (1 << i);
    }

    /* Determine if the node is a head */
    if (result->head[result->inlabel[id]] == NULL)
        result->head[result->inlabel[id]] = treenode;

    /* Recurse */
    second_transversal(treenode->sibling, result);
    second_transversal(treenode->child, result);
}

lca_result_t *lca_pre(tree_t *tree) {
    int n = tree->size;

    /* Setup the lca result */
    lca_result_t *result = lca_result_create(n);
    if (result == NULL)
        return NULL;

    /* Do the first transversal of the tree in order to determine preorder,
     * size and inlabel */
    first_transversal(tree->root, result);

    /* Setup the head array with a length of k determined during the first
     * transversal */
    result->head = calloc(result->k + 1, sizeof(treenode_t **));
    if (result->head == NULL)
        return NULL;

    /* Do the second transversal of the tree in order to determine ascendent,
     * level and head */
    result->ascendent[0] = 1 << (ceil_log2(n + 1) - 1);
    result->level[0] = 0;
    result->head[result->inlabel[0]] = tree->root;
    second_transversal(tree->root->child, result);

    return result;
}

void lca_result_destroy(lca_result_t *result) {
    if (result == NULL)
        return;

    free(result->preorder);
    free(result->size);
    free(result->inlabel);
    free(result->ascendent);
    free(result->level);
    free(result->head);
    free(result);
}

treenode_t *lca(lca_result_t *result, treenode_t *treenode1, treenode_t *treenode2) {
    int id1 = treenode1->id;
    int id2 = treenode2->id;

    /* Case 1: Same inlabel values, hence on the same path, so lca is the one
     * with lowest lvl */
    if (result->inlabel[id1] == result->inlabel[id2])
        return (result->level[id1] < result->level[id2]) ? treenode1 : treenode2;

    /* Case 2: Lots of math */
    int i = floor_log2(result->inlabel[id1] ^ result->inlabel[id2]);
    int b = (1 << i) * (result->inlabel[id1] >> i);
    b = b;

    int common = result->ascendent[id1] & result->ascendent[id2];
    common = (1 << i) * (common >> i);
    int j = floor_log2(common - (common & (common - 1)));
    int inlabelz = (1 << j) * (result->inlabel[id1] >> j);
    inlabelz = inlabelz;

    return NULL;
}

treenode_t *tree_lca_simple(treenode_t *treenode1, treenode_t *treenode2) {
    int n1 = treenode1->lvl+1;
    int n2 = treenode2->lvl+1;

    treenode_t *path1[n1];
    treenode_t *path2[n2];

    treenode_t *t = treenode1;
    for (int i = n1-1; i >= 0; i--) {
        path1[i] = t;
        t = t->parent;
    }
    t = treenode2;
    for (int i = n2-1; i >= 0; i--) {
        path2[i] = t;
        t = t->parent;
    }

    int i = 0;
    while (path1[i] == path2[i]) {
        i++;
        if (i >= n1 || i >= n2)
            break;
    }
    i--;

    return path1[i];
}

int floor_log2(int x) {
    int res = 0;
    while(x>>=1)
        res++;
    return res;
}

int ceil_log2(int x) {
    int res = 1;
    x--;
    while(x>>=1)
        res++;
    return res;
}
