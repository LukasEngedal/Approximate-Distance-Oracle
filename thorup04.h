#ifndef THORUP04_H
#define THORUP04_H

#include "graph.h"
#include "dijkstra.h"
#include "tree.h"

#define RESULT_INIT_P_SIZE 3

/* A covering_set_t object contains a list with the distance from each of the
 * vertices in the particular path to the particular vertex.
 * -1 indicates no connection.
 */
typedef struct covering_set_t {
    int n;                            /* Number of elements in d */
    int pid;                          /* ID of the path that the set covers */
    int *d;                           /* Distance to each vertex in the path */
} covering_set_t;

/* Each vertex has a vertex_result_t object, which contains the covering sets for
 * each of the encountered paths
 */
typedef struct vertex_result_t {
    int cap;                          /* Size of C */
    int n;                            /* Number of elements in C */
    treenode_t *final_call;
    covering_set_t **C;
} vertex_result_t;

/* The preprocess_result_t object has a list of vertex_result_t objects, with
 * n_v in total.
 * It also has a list of lists of integers, n_s in total, with each list of
 * integers being a copy of the d-array of a path.
 */
typedef struct preprocess_result_t {
    float eps;
    int n_v;                          /* Number of vertex_result_t objects in V */
    int n_p;                          /* Number of paths in P*/
    int cap_p;
    tree_t *recursion_tree;
    struct vertex_result_t **V;
    int **P;
} preprocess_result_t;

covering_set_t *covering_set_create(int n, int pid);
void covering_set_destroy(covering_set_t *c); /* TODO:  */

vertex_result_t *vertex_result_create(int n);
void vertex_result_destroy(); /* TODO:  */

preprocess_result_t *preprocess_result_create(float eps, int n);
void preprocess_result_destroy(preprocess_result_t *result); /* TODO:  */

/* Planar Separator Theorem */
path_t **pst(graph_t *graph);

void recursion2(graph_t *graph, path_t *path, preprocess_result_t *result);
preprocess_result_t *preprocess(graph_t *graph, float eps);

int query(preprocess_result_t *result);

#endif /* THORUP04_H */
