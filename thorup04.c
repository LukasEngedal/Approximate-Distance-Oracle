#include <time.h>
#include "thorup04.h"
#include "tests.h"

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
    for (int i = 0; i < n; i++) {
        C[i] = NULL;
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
        vertex_result = vertex_result_create(logn*3);
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

    tree_destroy(result->recursion_tree);

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

int tree_cycle_weight(edge_t *edge, edge_t *edge3) {
    vertex_t *v = edge->source;
    vertex_t *w = edge->target;

    treenode_t *node_v = (treenode_t *)v->node;
    treenode_t *node_w = (treenode_t *)w->node;

    /* If edge is a tree edge, inside = 0  per definition */
    if (node_v->parent == node_w || node_w->parent == node_v)
        return 0;

    path_t *path_v = tree_root_path(node_v);
    path_t *path_w = tree_root_path(node_w);

    int n_v = path_v->n;
    int n_w = path_w->n;

    if (n_v == 0 || n_w == 0)
        return 0;

    treenode_t *root_node = (treenode_t *)path_v->V[0]->node;
    int n = root_node->size;

    vertex_t *saved = path_v->V[n_v-2];

    /* We need to account for root paths sharing vertices */
    int start = 0;
    while (path_v->V[start] == path_w->V[start]) {
        start++;
        if (start == n_v || start == n_w)
            break;
    }
    start -= 1;

    /* path_v and path_w needs to differ by at least one vertex */
    if (n_w - start < 2 || n_w - start < 2) {
        path_destroy(path_v);
        path_destroy(path_w);
        return 0;
    }

    /* We remove the shared part of the root paths */
    path_t *p_v = path_split(path_v, start);
    path_destroy(path_v);
    path_t *p_w = path_split(path_w, start);
    path_destroy(path_w);

    /* We add the v to the end of path_w, and join the paths in order to make a
     * circle */
    path_expand(p_w, 1);
    p_w->V[p_w->n-1] = v;
    p_w->d[p_w->n-1] = p_w->d[p_w->n-2] + edge->w;

    path_t *path = path_join(p_v, p_w);
    path_destroy(p_v);
    path_destroy(p_w);

    /* We go around the circle, adding all the size values of tree edge nodes on
     * the inside of the circle */
    vertex_t *t;
    edge_t *e, *cc;
    treenode_t *node_s, *node_t;
    edge_t *prev = edge;
    vertex_t *s = v;
    int inside = 0;
    for (int i = 1; i < path->n; i++) {
        t = path->V[i];

        /* We determine the path edge */
        e = s->edge;
        for (int j = 0; j < s->n; j++) {
            if (e->target == t) {
                break;
            }
            e = e->c;
        }
        if (e->target != t) {
            printf("Could not find target edge\n");
            exit(EXIT_FAILURE);
        }

        /* We consider all the edges between e and prev */
        node_s = (treenode_t *)s->node;
        cc = e->cc;
        while (cc != prev) {
            node_t = (treenode_t *)cc->target->node;

            /* It it is a tree edge, we add the correct size */
            if (node_t->parent == node_s) {
                inside += node_t->size;
            } else if (node_s->parent == node_t) {
                inside += n - node_s->size;
            }

            cc = cc->cc;
        }

        prev = e->undirect;
        s = t;
    }

    if (edge3) {
        /* We determine whether the triangle is inside or outside */
        e = v->edge;
        while (e->target != saved) {
            e = e->c;
        }
        while (1) {
            if (e == edge)
                break;
            if (e == edge3->undirect) {
                inside = n - inside - n_v - n_w + start + 1;
                break;
            }

            e = e->cc;
        }
    }

    path_destroy(path);
    return inside;
}

path_t *tree_cycle_create(edge_t *e) {
    /* We create the separator path, by removing shared vertices from path_w,
     * flipping the result, extending it with v, and joining with path_v */
    treenode_t *node_v = (treenode_t *)e->source->node;
    treenode_t *node_w = (treenode_t *)e->target->node;
    path_t *path_v = tree_root_path(node_v);
    path_t *path_w = tree_root_path(node_w);

    int shared = 1;
    while (path_v->V[shared] == path_w->V[shared]) {
        shared++;
        if (shared == path_v->n-1 || shared == path_w->n-1)
            break;
    }
    shared -= 1;

    path_t *tmp = path_split(path_w, shared);
    path_destroy(path_w);

    path_expand(tmp, 1);
    tmp->V[tmp->n-1] = e->source;
    tmp->d[tmp->n-1] = tmp->d[tmp->n-2] + e->w;

    path_t *tmpf = path_flip(tmp);
    path_destroy(tmp);

    path_t *path = path_join(path_v, tmpf);
    path_destroy(path_v);
    path_destroy(tmpf);

    return path;
}

path_t **pst2(graph_t *graph) {
    float upper_limit = 0.60 * graph->n_v;
    float lower_limit = 0.40 * graph->n_v;

    int r, tries, i, inside;
    edge_t *e;
    tree_t *sp_tree;
    int found = 0;
    while (!found) {
        /* We create a shortest path tree */
        r = rand() % graph->cap_v;
        while(graph->V[r] == NULL)
            r = rand() % graph->cap_v;
        sp_tree = shortest_path_tree(graph, r);
        //printf("Tree root ID: %d\n", r);

        /* We look for an edge, if none if found in E*5 tries, we try another tree */
        tries = 0;
        while (!found) {
            if (tries >= graph->n_e * 5) {
                //printf("\n");
                upper_limit += 0.05 * graph->n_v;
                lower_limit -= 0.05 * graph->n_v;
                tree_destroy(sp_tree);
                break;
            }

            //printf("\rLooking for edge");
            tries++;
            i = rand() % graph->cap_e;
            e = graph->E[i];
            if (e == NULL)
                continue;

            inside = tree_cycle_weight(e, NULL);

            if (inside > lower_limit && inside < upper_limit)
                found = 1;
        }
    }
    //printf("\nEdge found\n");
    //printf("Inside: %d\n", inside);

    path_t **paths = malloc(sizeof(path_t *));
    paths[0] = tree_cycle_create(e);

    tree_destroy(sp_tree);
    return paths;
}

path_t **pst(graph_t *graph) {
    /* We create a shortest path tree */
    int r = rand() % graph->n_v;
    while(graph->V[r] == NULL)
        r = rand() % graph->cap_v;
    tree_t *bf_tree = breath_first_tree(graph, r);

    /* We look through our graph for a edge */
    edge_t *e1, *e2, *e3, *max_e;
    int i, inside1, inside2, inside3, max;
    while (1) {
        i = rand() % graph->cap_e;
        e1 = graph->E[i];
        if (e1 != NULL)
            break;
    }

    while (1) {
        //printf("pst\n");
        /* We find the other two edges creating a triangle with e1 */
        e2 = e1->undirect->cc;
        e3 = e2->undirect->cc;

        /* We check if it is a triangle */
        if (e1 != e3->undirect->cc) {
            printf("Why do we not have a triangle?\n");
            exit(EXIT_FAILURE);
        }

        /* We determine the cost inside the fundamental cycles of the edges */
        inside1 = tree_cycle_weight(e1, e3);
        inside2 = tree_cycle_weight(e2, e1);
        inside3 = tree_cycle_weight(e3, e2);
        //printf("inside1: %d, inside2: %d, inside3: %d\n", inside1, inside2, inside3);

        /* We determine the cost inside the cycle with the highest cost */
        max = (inside1 > inside2 ? inside1 : inside2);
        max_e = (inside1 > inside2 ? e1 : e2);
        max = (max > inside3 ? max : inside3);
        max_e = (max > inside3 ? max_e : e3);

        if (max > (graph->n_v >> 1)) {
            e1 = max_e->undirect;
        } else {
            break;
        }
    }

    /* We set up the array for the three paths */
    path_t **separators = malloc(sizeof(path_t *) * 3);
    if (separators == NULL)
        return NULL;

    separators[0] = tree_cycle_create(e1);
    separators[1] = tree_cycle_create(e2);
    separators[2] = tree_cycle_create(e3);

    tree_destroy(bf_tree);
    return separators;
}

void recursion2_helper(graph_t *graph, path_t *path, preprocess_result_t *result, int offset) {
    //printf("Recursion 2 helper\n");
    //printf("graph->n: %3d, path->n: %3d\n", graph->n_v, path->n);
    int n = path->n;
    if (n <= 2 || graph->n_v - n <= 2) {
        graph_destroy(graph);
        path_destroy(path);
        return;
    }

    /* We determine the middle vertex of the path and do a sssp calculation */
    int b = (n >> 1);
    dijkstra_result_t *result_b = dijkstra_sssp(graph, path->V[b]->id);
    int a_b = path->d[b] - path->d[0];
    int b_c = path->d[n - 1] - path->d[b];

    /* We reset the nodes and mark the path vertices */
    graph_reset_nodes(graph);
    for (int i = 0; i < n; i++) {
        path->V[i]->node = (void *)path;
    }

    /* We set up the two new graphs for the next step of the recursive call */
    graph_t *h1 = graph_copy(graph);
    graph_t *h2 = graph_copy(graph);

    /* We split the path into halves */
    path_t *path2 = path_split(path, b);

    /* We copy the two paths over to the graph copies */
    path_t *q1 = path_copy(h1, path);
    path_t *q2 = path_copy(h2, path2);
    path_destroy(path);
    path_destroy(path2);

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
        cset->d[offset + b] = dijkstra_query(result_b, vid);

        if (vertex->node != NULL)
            continue;

        v_a = cset->d[offset];
        v_b = cset->d[offset + b];
        v_c = cset->d[offset + n - 1];
        //printf("v_a: %3d, a_b: %3d, v_b: %3d, b_c: %3d, v_c: %3d\n", v_a, a_b, v_b, b_c, v_c);

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
    dijkstra_destroy(result_b);
    graph_destroy(graph);

    /* Finally we then recurse */
    recursion2_helper(h1, q1, result, offset);
    recursion2_helper(h2, q2, result, offset + b);
}

/* Recursion on a graph H and a path Q */
void recursion2(graph_t *graph, path_t *path, preprocess_result_t *result) {
    printf("Recursion 2, graph->n_v: %d, path->n: %d\n", graph->n_v, path->n);
    vertex_t *vertex;
    if (path->n <= 2 || graph->n_v - path->n < 2) {
        for (int i = 0; i < graph->cap_v; i++) {
            vertex = graph->V[i];
            if (vertex == NULL)
                continue;

            result->V[vertex->id]->n++;
        }

        graph_destroy(graph);
        path_destroy(path);
        return;
    }

    /* We store a copy of the d-array of the path in the result */
    preprocess_result_insert_path(result, path);

    /* We do a sssp calculation for each of the end points of the path */
    dijkstra_result_t *result_a = dijkstra_sssp(graph, path->V[0]->id);
    dijkstra_result_t *result_c = dijkstra_sssp(graph, path->V[path->n - 1]->id);

    /* We create the covering sets for this particular path for each of the
     * vertices in graph, and add the results from the sssp calculations */
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
        c->d[0] = dijkstra_query(result_a, vid);
        c->d[path->n - 1] = dijkstra_query(result_c, vid);

        vertex_result->C[vertex_result->n] = c;
        vertex_result->n++;
    }

    dijkstra_destroy(result_a);
    dijkstra_destroy(result_c);

    /* We start the recursive call */
    recursion2_helper(graph, path, result, 0);
}

void recursion1(graph_t *graph, preprocess_result_t *result, treenode_t *curr_call) {
    printf("Recursion 1, graph->n_v: %d\n", graph->n_v);
    //test_graph(graph);

    vertex_t *vertex;
    if (graph->n_v < 10) {
        for (int i = 0; i < graph->cap_v; i++) {
            vertex = graph->V[i];
            if (vertex == NULL)
                continue;

            result->V[vertex->id]->final_call = curr_call;
        }
        graph_destroy(graph);
        return;
    }

    /* We determine the three separator paths using the planar separator theorem */
    graph_t *pst_graph = graph_copy(graph);
    graph_triangulate(pst_graph);
    test_triangle(pst_graph);
    path_t **separators = pst(pst_graph);
    int n_p = 3; /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! HER !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
    if (separators == NULL)
        return;

    /* We do the distance recursion on the graph and each of the separator paths */
    graph_t *h;
    path_t *path;
    for (int i = 0; i < n_p; i++) {
        h = graph_copy(graph);
        path = path_copy(h, separators[i]);
        recursion2(h, path, result);
    }

    /* We remove the separator paths from the graph, and remove the separator
     * vertices */
    graph_t *graph_next = graph_copy(graph);
    vertex_t *v;
    for (int i = 0; i < n_p; i++) {
        path = separators[i];

        for (int j = 0; j < path->n; j++) {
            vertex = path->V[j];
            result->V[vertex->id]->final_call = curr_call;
            v = graph_remove_vertex(graph_next, graph_next->V[vertex->id]);
            vertex_destroy(v);
        }
        path_destroy(path);
    }
    free(separators);
    graph_destroy(graph);
    graph_destroy(pst_graph);

    /* We split the graph up into its connected components, and recurse on these */
    graph_t **graphs = graph_components(graph_next);

    int n_g = 0;
    while (graphs[n_g] != NULL) {
        n_g++;
    }

    graph_t *g;
    treenode_t *recurs_call;
    for (int i = 0; i < n_g; i++) {
        g = graphs[i];

        recurs_call = treenode_create(curr_call->value + n_p);
        tree_insert_treenode(result->recursion_tree, curr_call, recurs_call);
        recursion1(g, result, recurs_call);
    }
    free(graphs);
}

preprocess_result_t *thorup_preprocess(graph_t *graph, float eps) {
    preprocess_result_t *result = preprocess_result_create(eps, graph->n_v);
    int n_p = 3; /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! HER !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

    treenode_t *root = treenode_create(n_p);
    tree_t *tree = tree_create(root);
    result->recursion_tree = tree;

    recursion1(graph, result, root);

    return result;
}

int covering_set_dist(covering_set_t *c_s, covering_set_t *c_t, int *p_d) {
    int n_p = c_s->n;
    //printf("n_p: %2d, c_s->n: %2d, c_t->n: %2d\n", n_p, c_s->n, c_t->n);

    /* We set up the arrays */
    int d[n_p * 2];
    int id[n_p * 2];
    int pid[n_p * 2];
    for (int i = 0; i < n_p * 2; i++) {
        d[i] = -1;
        id[i] = -1;
        pid[i] = -1;
    }

    /* We merge the covering set d arrays */
    int j = 0;
    for (int i = 0; i < n_p; i++) {
        if (c_s->d[i] >= 0) {
            d[j] = c_s->d[i];
            id[j] = 0;
            pid[j] = i;
            j++;
        }
        if (c_t->d[i] >= 0) {
            d[j] = c_t->d[i];
            id[j] = 1;
            pid[j] = i;
            j++;
        }
    }

    /* We go through the merged array to find the shortest distance */
    int min = 1<<30;
    int dist;
    for (int i = 0; i < j-1; i++) {
        //printf("d: %3d, id: %d, pid: %2d, p_d: %3d\n", d[i], id[i], pid[i], p_d[pid[i]]);
        if (id[i] != id[i+1]) {
            dist = d[i] + d[i+1] + p_d[pid[i+1]] - p_d[pid[i]];
            if (dist < min) {
                min = dist;
            }
        }
    }

    return dist;
}

int thorup_query(preprocess_result_t *result, int s, int t) {
    vertex_result_t *result_s = result->V[s];
    vertex_result_t *result_t = result->V[t];

    if (result_s == NULL || result_t == NULL)
        printf("thorup_query: vertex_result is NULL!\n");

    treenode_t *lca_node = tree_lca_simple(result_s->final_call, result_t->final_call);
    int n = lca_node->value;
    //printf("node->value: %2d, result_s->n: %2d, result_t->n: %2d\n", n, result_s->n, result_t->n);

    int min = 1 << 30;
    covering_set_t *c_s, *c_t;
    for (int i = 0; i < n; i++) {
        c_s = result_s->C[i];
        c_t = result_t->C[i];
        if (c_s == NULL || c_t == NULL)
            continue;
        //printf("c_s->pid: %d, c_t->pid: %d\n", c_s->pid, c_t->pid);
        int *p_d = result->P[c_s->pid];

        int dist = covering_set_dist(c_s, c_t, p_d);
        if (dist < min)
            min = dist;
    }

    return min;
}
