#include <time.h>
#include <stdlib.h>
#include "graph.h"
#include "tree.h"

vertex_t *vertex_create(int id) {
    vertex_t *vertex = malloc(sizeof(vertex_t));
    if (vertex == NULL)
        return NULL;

    vertex->id = id;
    vertex->n = 0;
    vertex->edge = NULL;
    vertex->node = NULL;

    return vertex;
}

void vertex_destroy(vertex_t *vertex) {
    if (vertex != NULL) {
        free(vertex);
    }
}

edge_t *edge_create(vertex_t *v, vertex_t *u, int w) {
    if (w < 0) {
        return NULL;
    }

    edge_t *edge = malloc(sizeof(edge_t));
    if (edge == NULL)
        return NULL;

    edge->id = -1;
    edge->source = v;
    edge->target = u;
    edge->w = w;
    edge->c = NULL;
    edge->cc = NULL;
    edge->undirect = NULL;

    return edge;
}

void edge_destroy(edge_t *edge) {
    if (edge != NULL)
        free(edge);
}

graph_t *graph_create() {
    graph_t *graph = malloc(sizeof(graph_t));
    if (graph == NULL)
        return NULL;

    graph->n_v = 0;
    graph->n_e = 0;
    graph->i_e = 0;
    graph->cap_v = GRAPH_INIT_V_SIZE;
    graph->cap_e = GRAPH_INIT_E_SIZE;

    vertex_t **V = malloc(sizeof(vertex_t *) * graph->cap_v);
    if (V == NULL) {
        free(graph);
        return NULL;
    }
    edge_t **E = malloc(sizeof(edge_t *) * graph->cap_e);
    if (E == NULL) {
        free(V);
        free(graph);
        return NULL;
    }

    for (int i = 0; i < graph->cap_v; i++) {
        V[i] = NULL;
    }
    for (int i = 0; i < graph->cap_e; i++) {
        E[i] = NULL;
    }

    graph->V = V;
    graph->E = E;
    return graph;
}

void graph_destroy(graph_t *graph) {
    if (graph == NULL)
        return;

    for (int i = 0; i < graph->cap_v; i++) {
        vertex_destroy(graph->V[i]);
    }
    free(graph->V);

    for (int i = 0; i < graph->cap_e; i++) {
        edge_destroy(graph->E[i]);
    }
    free(graph->E);

    free(graph);
}

graph_t *graph_copy(graph_t *graph) {
    /* We create the copy and expand the E array */
    graph_t *gc = graph_create();
    gc->i_e = graph->i_e;

    edge_t **new_E = realloc(gc->E, sizeof(edge_t *) * graph->cap_e);
    if (new_E == NULL)
        return NULL;
    gc->E = new_E;
    gc->cap_e = graph->cap_e;
    for (int i = 0; i < gc->cap_e; i++) {
        gc->E[i] = NULL;
    }

    /* We copy over all the vertices */
    vertex_t *vertex;
    vertex_t *v_copy;
    for (int i = 0; i < graph->cap_v; i++) {
        vertex = graph->V[i];
        if (vertex == NULL)
            continue;

        v_copy = vertex_create(vertex->id);
        graph_insert_vertex(gc, v_copy);
        //v_copy->n = vertex->n;
    }

    /* We copy over all the edges */
    edge_t *e, *ec;
    for (int i = 0; i < graph->cap_v; i++) {
        vertex = graph->V[i];
        if (vertex == NULL)
            continue;

        e = vertex->edge;
        for (int j = 0; j < vertex->n; j++) {
            ec = edge_create(gc->V[e->source->id], gc->V[e->target->id], e->w);
            ec->id = e->id;
            graph_insert_edge(gc, ec);

            e = e->c;
        }
    }

    /* We set the edge->undirect pointers */
    for (int i = 0; i < graph->cap_e; i++) {
        e = graph->E[i];
        ec = gc->E[i];
        if (e == NULL)
            continue;

        ec->undirect = gc->E[e->undirect->id];
    }
    /* We adjust the graph copy E array */
    /* edge_t **new_E = malloc(sizeof(edge_t *) * graph->cap_e); */
    /* free(gc->E); */
    /* gc->E = new_E; */
    /* gc->cap_e = graph->cap_e; */


    /* edge_t *edge; */
    /* edge_t *e_copy; */
    /* for (int i = 0; i < graph->cap_e; i++) { */
    /*     edge = graph->E[i]; */
    /*     if (edge == NULL) { */
    /*         gc->E[i] = NULL; */
    /*         continue; */
    /*     } */

    /*     e_copy = edge_create(gc->V[edge->source->id], gc->V[edge->target->id], edge->w); */
    /*     gc->E[i] = e_copy; */
    /*     e_copy->id = i; */
    /*     gc->n_e++; */
    /* } */

    /* /\* We set the vertex edge pointers *\/ */
    /* for (int i = 0; i < graph->cap_v; i++) { */
    /*     vertex = graph->V[i]; */
    /*     if (vertex == NULL) */
    /*         continue; */

    /*     v_copy = gc->V[i]; */
    /*     v_copy->edge = gc->E[vertex->edge->id]; */
    /* } */

    /* /\* We set the edge c and cc pointers *\/ */
    /* for (int i = 0; i < graph->cap_e; i++) { */
    /*     edge = graph->E[i]; */
    /*     if (edge == NULL) */
    /*         continue; */

    /*     e_copy = gc->E[edge->id]; */
    /*     e_copy->c = gc->E[edge->c->id]; */
    /*     e_copy->cc = gc->E[edge->cc->id]; */
    /* } */

    return gc;
}

void graph_reset_nodes(graph_t *graph) {
    vertex_t *v;
    for (int i = 0; i < graph->cap_v; i++) {
        v = graph->V[i];
        if (v == NULL)
            continue;

        v->node = NULL;
    }
}

path_t *path_create(int n) {
    path_t *path = malloc(sizeof(path_t));
    if (path == NULL)
        return NULL;

    path->V = malloc(sizeof(vertex_t *) * n);
    if (path->V == NULL) {
        free(path);
        return NULL;
    }
    path->d = malloc(sizeof(int) * n);
    if (path->d == NULL) {
        free(path->V);
        free(path);
        return NULL;
    }

    path->n = n;

    return path;
}

void path_destroy(path_t *path) {
    if (path == NULL)
        return;

    free(path->d);
    free(path->V);
    free(path);
}

/* Copies the given path to the given graph, assuming they align */
path_t *path_copy(graph_t *graph, path_t *path){
    path_t *pcopy = path_create(path->n);
    for (int i = 0; i < path->n; i++) {
        pcopy->V[i] = graph->V[path->V[i]->id];
        pcopy->d[i] = path->d[i];
    }

    return pcopy;
}

/* Expands the given path with n more vertices at the end of the path */
int path_expand(path_t *path, int n) {
    int new_n = path->n + n;
    vertex_t **new_V = realloc(path->V, sizeof(vertex_t *) * new_n);
    if (new_V == NULL) {
        printf("path_expand: no more memory!\n");
        return -1;
    }

    int *new_d = realloc(path->d, sizeof(int) * new_n);
    if (new_d == NULL) {
        printf("path_expand: no more memory!\n");
        return -1;
    }

    path->V = new_V;
    path->d = new_d;
    path->n = new_n;

    return 0;
}

/* Inserts the given vertex into the given graph, placing it at a position in then
 * graphs list of vertices equal to it's id. Does not check for overwriting */
int graph_insert_vertex(graph_t *graph, vertex_t *vertex) {
    int id = vertex->id;
    if (id >= graph->cap_v) {
        int old_cap = graph->cap_v;
        while (graph->cap_v <= id) {
            graph->cap_v *= 2;
        }
        vertex_t **new_V = realloc(graph->V, sizeof(vertex_t *) * graph->cap_v);
        if (new_V == NULL)
            return -1;

        for (int i = old_cap; i < graph->cap_v; i++) {
            new_V[i] = NULL;
        }

        graph->V = new_V;
    }
    graph->V[id] = vertex;
    graph->n_v++;

    return 0;
}

/* Removes the given vertex from the given graph, deleting all edges, but not
 * the vertex itself */
vertex_t *graph_remove_vertex(graph_t *graph, vertex_t *vertex) {
    if (vertex == NULL)
        return NULL;

    if (graph->V[vertex->id] != vertex) {
        printf("WTF?\n");
        exit(EXIT_FAILURE);
    }

    graph->V[vertex->id] = NULL;
    graph->n_v--;

    int n = vertex->n;
    if (n == 0)
        return 0;

    edge_t *edge = vertex->edge;
    edge_t *next;
    if (edge == NULL) {
        printf("Why is edge NULL when v->n is not 0?\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        next = edge->c;

        graph_remove_edge(graph, edge);
        graph_remove_edge(graph, edge->undirect);
        edge_destroy(edge->undirect);
        edge_destroy(edge);

        edge = next;
    }

    return vertex;
}

void vertex_insert_edge(vertex_t *vertex, edge_t *edge) {
    if (edge->source != vertex) {
        printf("vertex_insert_edge: edge->source->id does not match vertex->id\n");
        exit(EXIT_FAILURE);
    }
    vertex->n++;

    edge_t *first = vertex->edge;
    if (first == NULL) {
        vertex->edge = edge;
        edge->c = edge;
        edge->cc = edge;
        return;
    }

    first->cc->c = edge;
    edge->cc = first->cc;

    first->cc = edge;
    edge->c = first;
}

int graph_insert_edge(graph_t *graph, edge_t *edge) {
    if (edge->id == -1) {
        edge->id = graph->i_e;
        graph->i_e++;
    }

    if (edge->id >= graph->cap_e) {
        int old = graph->cap_e;
        while (edge->id >= graph->cap_e)
            graph->cap_e *= 2;
        edge_t **new_E = realloc(graph->E, sizeof(edge_t *) * graph->cap_e);
        if (new_E == NULL)
            return -1;

        for (int i = old; i < graph->cap_e; i++) {
            new_E[i] = NULL;
        }

        graph->E = new_E;
    }

    if (graph->E[edge->id] != NULL)
        printf("Pas på!\n");
    graph->E[edge->id] = edge;
    graph->n_e++;

    vertex_insert_edge(edge->source, edge);

    return 0;
}

void vertex_insert_edge_at(vertex_t *vertex, edge_t *edge, edge_t *next) {
    vertex->n++;

    next->cc->c = edge;
    edge->cc = next->cc;

    next->cc = edge;
    edge->c = next;
}

int graph_insert_edge_at(graph_t *graph, edge_t *edge, edge_t *next) {
    if (edge->id == -1) {
        edge->id = graph->i_e;
        graph->i_e++;
    }

    if (edge->id >= graph->cap_e) {
        int old = graph->cap_e;
        while (edge->id >= graph->cap_e)
            graph->cap_e *= 2;
        edge_t **new_E = realloc(graph->E, sizeof(edge_t *) * graph->cap_e);
        if (new_E == NULL)
            return -1;

        for (int i = old; i < graph->cap_e; i++) {
            new_E[i] = NULL;
        }

        graph->E = new_E;
    }

    if (graph->E[edge->id] != NULL)
        printf("Pas på!\n");
    graph->E[edge->id] = edge;
    graph->n_e++;

    vertex_insert_edge_at(edge->source, edge, next);

    return 0;
}

/* Removes the given edge from the given vertex's linked list of edges. Does not
 * delete anything */
int vertex_remove_edge(vertex_t *vertex, edge_t *edge) {
    edge_t *e = vertex->edge;
    int found = 0;
    for (int i = 0; i < vertex->n; i++) {
        if (e == edge) {
            found += 1;
        }
        e = e->c;
    }
    if (!found) {
        return -1;
        printf("D'oh!\n");
    }

    vertex->n--;
    if (vertex->edge == edge) {
        if (edge->c == edge) {
            vertex->edge = NULL;
            return 0;
        }
        vertex->edge = edge->c;
    }

    edge->c->cc = edge->cc;
    edge->cc->c = edge->c;
    return 0;
}

int graph_remove_edge(graph_t *graph, edge_t *edge) {
    int ret = vertex_remove_edge(edge->source, edge);
    graph->E[edge->id] = NULL;
    graph->n_e--;

    return ret;
}

void graph_undirect(graph_t *graph) {
    vertex_t *s, *t;
    edge_t *e, *u;
    for (int i = 0; i < graph->cap_e; i++) {
        e = graph->E[i];
        if (e == NULL || e->undirect != NULL)
            continue;

        s = e->source;
        t = e->target;

        u = t->edge;
        for (int j = 0; j < t->n; j++) {
            if (u->target == s) {
                u->undirect = e;
                e->undirect = u;
                break;
            }
            u = u->c;
        }
    }
}

/* Split the given path at vertex i, returning the part of the path from i to n */
path_t *path_split(path_t *path, int i) {
    if (path->n == 0)
        return NULL;

    path_t *path2 = path_create(path->n - i);

    int offset = path->d[i];
    for (int j = i; j < path->n; j++) {
        path2->V[j - i] = path->V[j];
        path2->d[j - i] = path->d[j] - offset;
    }

    vertex_t **new_V = realloc(path->V, sizeof(vertex_t *) * (i + 1));
    if (new_V == NULL) {
        free(path2);
        return NULL;
    }
    int *new_d = realloc(path->d, sizeof(int) * (i + 1));
    if (new_d == NULL) {
        free(new_V);
        free(path2);
        return NULL;
    }

    path->V = new_V;
    path->d = new_d;
    path->n = i + 1;

    return path2;
}

path_t *path_join(path_t *path1, path_t *path2) {
    int n1 = path1->n;
    int n2 = path2->n;

    path_t *p1, *p2;
    int flip1 = 0;
    int flip2 = 0;
    if (path1->V[0] == path2->V[0]) {
        p1 = path_flip(path1);
        flip1 = 1;
        p2 = path2;
    } else if (path1->V[0] == path2->V[n2-1]) {
        p1 = path_flip(path1);
        flip1 = 1;
        p2 = path_flip(path2);
        flip2 = 1;
    } else if (path1->V[n1-1] == path2->V[0]) {
        p1 = path1;
        p2 = path2;
    } else if (path1->V[n1-1] == path2->V[n2-1]) {
        p1 = path1;
        p2 = path_flip(path2);
        flip2 = 1;
    } else {
        return NULL;
    }

    path_t *path_result = path_create(n1 + n2 - 1);

    for (int i = 0; i < n1; i++) {
        path_result->V[i] = p1->V[i];
        path_result->d[i] = p1->d[i];
    }

    int d1 = p1->d[n1-1];
    for (int i = 0; i < n2 - 1; i++) {
        path_result->V[n1 + i] = p2->V[i + 1];
        path_result->d[n1 + i] = p2->d[i + 1] + d1;
    }

    if (flip1)
        path_destroy(p1);
    if (flip2)
        path_destroy(p2);

    return path_result;
}

/* Flips a path and returns the result */
path_t *path_flip(path_t *path) {
    int n = path->n;
    path_t *path_f = path_create(n);
    path_f->V[0] = path->V[n-1];
    path_f->d[0] = 0;

    for (int i = 1; i < n; i++) {
        path_f->V[i] = path->V[n-i-1];
        path_f->d[i] = path_f->d[i-1] + path->d[n-i] - path->d[n-i-1];
    }

    return path_f;
}

/* Removes all the vertices of the given path from the given graph */
int graph_remove_path(graph_t *graph, path_t *path) {
    for (int i = 0; i < path->n; i++) {
        graph_remove_vertex(graph, path->V[i]);
    }

    return 0;
}

void graph_components_helper(vertex_t *v, int graph, int *visited, int *belong) {
    visited[v->id] = 1;
    belong[v->id] = graph;

    vertex_t *u;
    edge_t *edge = v->edge;
    for (int i = 0; i < v->n; i++) {
        u = edge->target;
        if (!visited[u->id]) {
            graph_components_helper(u, graph, visited, belong);
        }
        edge = edge->c;
    }
}

graph_t **graph_components(graph_t *graph) {
    int cap_v = graph->cap_v;
    int n_graphs = 0;
    int belong[cap_v];
    int visited[cap_v];
    memset(visited, 0, sizeof(int) * cap_v);

    /* We determine the number of graphs and which graph each vertex belongs to */
    vertex_t *v;
    for (int i = 0; i < cap_v; i++) {
        v = graph->V[i];
        if (v == NULL)
            continue;

        if (!visited[i]) {
            graph_components_helper(v, n_graphs, visited, belong);
            n_graphs++;
        }
    }

    /* We set up the array of graphs, and end with a NULL in order to indicate array length */
    graph_t **graphs = malloc(sizeof(graph_t *) * (n_graphs + 1));
    graph_t *g;
    for (int i = 0; i < n_graphs; i++) {
        g = graph_create();
        g->i_e = graph->i_e;

        edge_t **new_E = realloc(g->E, sizeof(edge_t *) * graph->cap_e);
        if (new_E == NULL)
            return NULL;
        g->E = new_E;
        g->cap_e = graph->cap_e;
        for (int i = 0; i < g->cap_e; i++) {
            g->E[i] = NULL;
        }

        graphs[i] = g;
    }
    graphs[n_graphs] = NULL;

    /* We insert the vertices into their respective graphs */
    for (int i = 0; i < cap_v; i++) {
        v = graph->V[i];
        if (v == NULL)
            continue;

        graph_insert_vertex(graphs[belong[i]], v);
    }

    /* We insert the edges into their respective graphs */
    edge_t *e;
    for (int i = 0; i < graph->cap_e; i++) {
        e = graph->E[i];
        if (e == NULL)
            continue;

        g = graphs[belong[e->source->id]];
        g->E[e->id] = e;
        g->n_e++;
    }

    /* We clean up the old graph */
    free(graph->V);
    free(graph->E);
    free(graph);

    return graphs;
}


graph_t *graph_tree_reduce(graph_t *graph) {
    edge_t *e;
    vertex_t *s, *t;
    treenode_t *sn, *tn;
    for (int i = 0; i < graph->cap_e; i++) {
        e = graph->E[i];
        if (e == NULL)
            continue;

        s = e->source;
        t = e->target;
        sn = (treenode_t *)s->node;
        tn = (treenode_t *)t->node;
        if (sn == NULL || tn == NULL)
            return NULL;
        if (sn->parent == tn || tn->parent == sn)
            continue;

        graph_remove_edge(graph, e);
        edge_destroy(e);
        graph->E[i] = NULL;
    }

    return graph;
}


graph_t *graph_from_file_M(char *filename) {
    FILE *file;
    vertex_t *v;
    vertex_t *u;
    edge_t *e;
    char c;
    int n = 0;

    graph_t *graph = graph_create();

    file = fopen(filename, "r");
    if (file == NULL)
        return NULL;

    /* We read the first word in order to construct the first vertex */
    fgetc(file);
    v = vertex_create(n);
    graph_insert_vertex(graph, v);
    n++;
    fgetc(file);

    /* We then read the rest of the first line of text in order to create all of the remaining vertices */
    while (1) {
        c = fgetc(file);
        u = vertex_create(n);
        graph_insert_vertex(graph, u);

        if (c != '0') {
            e = edge_create(v, u, c - '0');
            graph_insert_edge(graph, e);
        }
        n++;
        if ((fgetc(file)) == '\n') {
            break;
        }
    }


    /* We then read the rest of the file and add all the remaining edges */
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < n; j++) {
            c = fgetc(file);
            if (c != '0') {
                e = edge_create(graph->V[i], graph->V[j], c - '0');
                graph_insert_edge(graph, e);
            }
            fgetc(file);
        }
    }

    fclose(file);
    graph_undirect(graph);
    return graph;
}

void graph_triangulate(graph_t *graph) {
    edge_t *e1, *e2, *e3, *e4, *new, *new_u;
    for (int i = 0; i < graph->cap_e; i++) {
        e1 = graph->E[i];
        if (e1 == NULL)
            continue;

        e2 = e1->undirect->cc;
        e3 = e2->undirect->cc;
        e4 = e3->undirect->cc;

        if (e1 != e4) {
            new = edge_create(graph->V[e1->source->id], graph->V[e3->source->id], 0);
            graph_insert_edge_at(graph, new, e1->c);

            new_u = edge_create(graph->V[e3->source->id], graph->V[e1->source->id], 0);
            graph_insert_edge_at(graph, new_u, e3->c);

            new->undirect = new_u;
            new_u->undirect = new;
        }

        e2 = e1->undirect->c;
        e3 = e2->undirect->c;
        e4 = e3->undirect->c;

        if (e1 != e4) {
            new = edge_create(graph->V[e1->source->id], graph->V[e3->source->id], 0);
            graph_insert_edge_at(graph, new, e1);

            new_u = edge_create(graph->V[e3->source->id], graph->V[e1->source->id], 0);
            graph_insert_edge_at(graph, new_u, e3);

            new->undirect = new_u;
            new_u->undirect = new;
        }
    }
}

/* Generates a graph in the form of a triangulated grid. Assumes x, y > 2 */
graph_t *graph_generate(int x, int y, int maxw) {
    int n = x * y;
    graph_t *graph = graph_create();

    srand(time(NULL));
    int max = maxw - 1;

    vertex_t *v;
    for (int i = 0; i < n; i++) {
        v = vertex_create(i);
        graph_insert_vertex(graph, v);
    }

    edge_t *e;
    for (int i = 0; i < n; i++) {
        v = graph->V[i];
        /* E edge */
        if (i % x != x - 1) {
            e = edge_create(v, graph->V[i+1], 0);
            graph_insert_edge(graph, e);
        }

        /* SE edge */
        if (i % x != x - 1 && i < (n - x)) {
            e = edge_create(v, graph->V[i+x+1], 0);
            graph_insert_edge(graph, e);
        }

        /* S edge */
        if (i < (n - x)) {
            e = edge_create(v, graph->V[i+x], 0);
            graph_insert_edge(graph, e);
        }

        /* W edge */
        if (i % x != 0) {
            e = edge_create(v, graph->V[i-1], 0);
            graph_insert_edge(graph, e);
        }

        /* NW edge */
        if (i % x != 0 && i >= x) {
            e = edge_create(v, graph->V[i-x-1], 0);
            graph_insert_edge(graph, e);
        }

        /* N edge */
        if (i >= x) {
            e = edge_create(v, graph->V[i-x], 0);
            graph_insert_edge(graph, e);
        }
    }

    /* We triangulate the outer face */
    graph_undirect(graph);
    graph_triangulate(graph);
    /* int si = rand() % (x - 4) + 2; */
    /* vertex_t *s = graph->V[si]; */
    /* edge_t *c; */
    /* /\* North side part 1 *\/ */
    /* for (int i = si-2; i >= 0; i--) { */
    /*     v = graph->V[i]; */
    /*     e = edge_create(s, v, 0); */
    /*     graph_insert_edge(graph, e); */
    /*     e = edge_create(v, s, 0); */
    /*     graph_insert_edge(graph, e); */
    /* } */

    /* /\* West side *\/ */
    /* for (int i = 1; i < y; i++) { */
    /*     v = graph->V[i*x]; */
    /*     e = edge_create(s, v, 0); */
    /*     graph_insert_edge(graph, e); */
    /*     e = edge_create(v, s, 0); */
    /*     c = v->edge->cc; */
    /*     graph_insert_edge_at(graph, e, c); */
    /* } */

    /* /\* South side *\/ */
    /* for (int i = 1; i < x-1; i++) { */
    /*     v = graph->V[n-x+i]; */
    /*     e = edge_create(s, v, 0); */
    /*     graph_insert_edge(graph, e); */
    /*     e = edge_create(v, s, 0); */
    /*     c = v->edge->c; */
    /*     graph_insert_edge_at(graph, e, c); */
    /* } */

    /* /\* East side *\/ */
    /* for (int i = 0; i < y; i++) { */
    /*     v = graph->V[n-1-i*x]; */
    /*     e = edge_create(s, v, 0); */
    /*     graph_insert_edge(graph, e); */
    /*     e = edge_create(v, s, 0); */
    /*     c = v->edge; */
    /*     graph_insert_edge_at(graph, e, c); */
    /* } */

    /* /\* North side part 2 *\/ */
    /* for (int i = x-2; i > si+1; i--) { */
    /*     v = graph->V[i]; */
    /*     e = edge_create(s, v, 0); */
    /*     graph_insert_edge(graph, e); */
    /*     e = edge_create(v, s, 0); */
    /*     graph_insert_edge(graph, e); */
    /* } */

    /* graph_undirect(graph); */

    edge_t *e_undirect;
    for (int i = 0; i < graph->n_e; i++) {
        e = graph->E[i];
        e_undirect = e->undirect;

        if (e->w == 0 && e_undirect->w == 0) {
            e->w = rand() % max + 1;
            e_undirect->w = e->w;
        }
        if (e->w == 0) {
            e->w = e_undirect->w;
            printf("Her?\n");
        }
        if(e_undirect->w == 0) {
            e_undirect->w = e->w;
            printf("Her??\n");
        }
    }

    return graph;
}

void graph_print(graph_t *graph) {
    vertex_t *v;
    vertex_t *u;
    edge_t *e;
    int w;

    printf("Graph:\n");
    // For each node in the graph print a line
    for (int i = 0; i < graph->cap_v; i++) {
        v = graph->V[i];
        if (v == NULL)
            continue;

        // For each node in the graph, print the weight if there is an edge
        for (int j = 0; j < graph->cap_v; j++) {
            u = graph->V[j];
            if (u == NULL)
                continue;

            e = v->edge;
            w = 0;

            for (int k = 0; k < v->n; k++) {
                if (u == e->target) {
                    w = e->w;
                }
                e = e->c;
            }
            printf("%d ", w);
        }
        printf("\n");
    }
}

void path_print(path_t *path) {
    printf("Path:\n");
    for (int i = 0; i < path->n; i++) {
        printf("ID: %4d, d: %4d\n", path->V[i]->id, path->d[i]);
    }
}
