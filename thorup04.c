#include "thorup04.h"

covering_set_t *covering_set_create(int n, int pid) {
    covering_set_t *c = malloc(sizeof(covering_set_t));
    if (c == NULL)
        return NULL;

    int *d = malloc(sizeof(int) * n);
    if (d == NULL) {
        free(c);
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        d[i] = -1;
    }

    c->d = d;
    c->n = n;
    c->pid = pid;
    return c;
}

void covering_set_destroy(covering_set_t *c) {
    if (c == NULL)
        return;

    free(c->d);
    free(c);
}

vertex_result_t *vertex_result_create(int n) {
    vertex_result_t *vertex_result = malloc(sizeof(vertex_result_t));
    if (vertex_result == NULL)
        return NULL;

    covering_set_t **C = malloc(sizeof(covering_set_t *) * n);
    if (C == NULL) {
        free(vertex_result);
        return NULL;
    }

    vertex_result->C = C;
    vertex_result->cap = n;
    vertex_result->n = 0;
    return vertex_result;
}

void vertex_result_destroy(vertex_result_t *v) {
    for (int i = 0; i < v->n; i++) {
        covering_set_destroy(v->C[i]);
    }
    free(v->C);
    free(v);
}

preprocess_result_t *preprocess_result_create(float eps, int n) {
    int logn = ceil_log2(n);

    preprocess_result_t *result = malloc(sizeof(preprocess_result_t));
    if (result == NULL)
        return NULL;

    result->n_v = n;
    result->n_p = 0;
    result->cap_p = RESULT_INIT_P_SIZE;

    vertex_result_t **V = malloc(sizeof(vertex_result_t *) * result->n_v);
    if (V == NULL) {
        free(result);
        return NULL;
    }
    int **P = malloc(sizeof(int *) * result->cap_p);
    if (P == NULL) {
        free(V);
        free(result);
        return NULL;
    }

    result->V = V;
    result->P = P;
    result->eps = eps;
    result->recursion_tree = NULL;

    vertex_result_t *vertex_result;
    for (int i = 0; i < result->n_v; i++) {
        vertex_result = vertex_result_create(logn);
        result->V[i] = vertex_result;
    }
    /* TODO: fix vertex_result == NULL */

    return result;
}

void preprocess_result_destroy(preprocess_result_t *result) {
    for (int i = 0; i < result->n_v; i++) {
        vertex_result_destroy(result->V[i]);
    }
    free(result->V);

    for (int i = 0; i < result->n_p; i++) {
        free(result->P[i]);
    }
    free(result->P);

    free(result);
}

int preprocess_result_insert_path(preprocess_result_t *result, path_t *path) {
    if (result->n_p >= result->cap_p) {
        result->cap_p *= 3;
        int **new_P = realloc(result->P, sizeof(int *) * result->cap_p);
        if (new_P == NULL)
            return -1;

        result->P = new_P;
    }

    int *d = malloc(sizeof(int) * path->n);
    if (d == NULL)
        return -1;

    for (int i = 0; i < path->n; i++) {
        d[i] = path->d[i];
    }

    result->P[result->n_p] = d;
    result->n_p++;
    return 0;
}

path_t **pst(graph_t *graph) {
    graph = graph;
    return NULL;
}

void recursion2_helper(graph_t *graph, path_t *path, preprocess_result_t *result, int offset) {
    printf("Her!\n");
    int n = path->n;
    if (n <= 2)
        return;

    /* We determine the middle vertex of the path and do a sssp calculation */
    int b = (n >> 1);
    dijkstra_result_t *result_b = dijkstra_sssp(graph, path->V[b]->id);
    int a_b = path->d[b] - path->d[0];
    int b_c = path->d[n - 1] - path->d[b];

    /* We set up the two new graphs for the next step of the recursive call */
    graph_t *h1 = graph_copy(graph);
    graph_t *h2 = graph_copy(graph);

    /* For each vertex we add the result from the sssp calculation to the
     * covering set, and remove the vertex from either of the graphs if required. */
    vertex_t *vertex, *v1, *v2;
    int vid;
    vertex_result_t *vertex_result;
    covering_set_t *cset;
    int v_a, v_b, v_c;
    int eps = result->eps;

    for (int i = 0; i < graph->cap_v; i++) {
        vertex = graph->V[i];
        if (vertex == NULL)
            continue;

        vid = vertex->id;
        vertex_result = result->V[vid];

        cset = vertex_result->C[vertex_result->n - 1];
        cset->d[offset + b] = result_b->dist[vid];

        v_a = cset->d[offset];
        v_b = cset->d[offset + b];
        v_c = cset->d[offset + n - 1];

        /* We check if either v_a or v_b epsilon-covers the other */
        if (v_a >= (1 - eps) * v_b + a_b ||
            v_b >= (1 - eps) * v_a + a_b) {
            v1 = h1->V[vid];
            graph_remove_vertex(h1, v1);
            vertex_destroy(v1);
        }
        /* We check if either v_b or v_c epsilon-covers the other */
        if (v_c >= (1 - eps) * v_b + b_c ||
            v_b >= (1 - eps) * v_c + b_c) {
            v2 = h2->V[vid];
            graph_remove_vertex(h2, v2);
            vertex_destroy(v2);
        }
    }
    free(result_b);
    graph_destroy(graph);

    /* We split the path into halves */
    path_t *path2 = path_split(path, b);

    /* Finally we then recurse */
    recursion2_helper(h1, path, result, offset);
    recursion2_helper(h2, path2, result, offset + b);
}

/* Recursion on a graph H and a path Q */
void recursion2(graph_t *graph, path_t *path, preprocess_result_t *result) {
    /* We store a copy of the d-array of the path in the result */
    preprocess_result_insert_path(result, path);

    /* We do a sssp calculation for each of the end points of the path */
    dijkstra_result_t *result_a = dijkstra_sssp(graph, path->V[0]->id);
    dijkstra_result_t *result_c = dijkstra_sssp(graph, path->V[path->n - 1]->id);

    /* We create the covering sets for this particular path for each of the
     * vertices in graph, and add the results from the sssp calculations */
    vertex_t *vertex;
    int vid;
    vertex_result_t *vertex_result;
    covering_set_t *c;

    for (int i = 0; i < graph->cap_v; i++) {
        vertex = graph->V[i];
        if (vertex == NULL)
            continue;

        vid = vertex->id;
        vertex_result = result->V[vid];

        c = covering_set_create(path->n, result->n_p - 1);
        c->d[0] = result_a->dist[vid];
        c->d[path->n - 1] = result_c->dist[vid];

        vertex_result->C[vertex_result->n] = c;
        vertex_result->n++;
    }

    free(result_a);
    free(result_c);

    /* We start the recursive call */
    recursion2_helper(graph, path, result, 0);
}

void recursion1(graph_t *graph, preprocess_result_t *result, treenode_t *curr_call) {
    /* We determine the three separator paths using the planar separator theorem */
    path_t **separators = pst(graph);
    if (separators == NULL)
        return;

    /* We do the distance recursion on the graph and each of the separator paths */
    graph_t *h;
    for (int i = 0; i < 3; i++) {
        h = graph_copy(graph);
        recursion2(h, separators[i], result);
    }

    /* We remove the separator paths from the graph, and update the separator
     * vertices */
    path_t *path;
    vertex_t *vertex;
    for (int i = 0; i < 3; i++) {
        path = separators[i];
        graph_remove_path(graph, path);

        for (int j = 0; j < path->n; j++) {
            vertex = path->V[j];
            result->V[vertex->id]->final_call = curr_call;
            vertex_destroy(vertex);
        }
        path_destroy(path);
    }
    free(separators);

    /* We split the graph up into its connected components, and recurse on these */
    graph_t **graphs = graph_components(graph);

    int i = 0;
    graph_t *g = graphs[0];
    treenode_t *recurs_call;
    while (g != NULL) {
        recurs_call = treenode_create(curr_call->value + 3);
        tree_insert_treenode(result->recursion_tree, curr_call, recurs_call);
        recursion1(g, result, recurs_call);

        i++;
        g = graphs[i];
    }
    free(graphs);
}

preprocess_result_t *preprocess(graph_t *graph, float eps) {
    preprocess_result_t *result = preprocess_result_create(eps, graph->n_v);

    treenode_t *root = treenode_create(3);
    tree_t *tree = tree_create(root);
    result->recursion_tree = tree;

    recursion1(graph, result, root);

    return result;
}

int query(preprocess_result_t *result) {
    result = result;
    return 0;
}
