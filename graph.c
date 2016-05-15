#include "graph.h"

vertex_t *vertex_create(int id) {
    vertex_t *vertex = malloc(sizeof(vertex_t));
    if (vertex == NULL)
        return NULL;

    vertex->cap = VERTEX_INIT_E_SIZE;
    edge_t **E = malloc(sizeof(edge_t *) * vertex->cap);
    if (E == NULL) {
        free(vertex);
        return NULL;
    }

    vertex->id = id;
    vertex->n = 0;
    vertex->node = NULL;
    vertex->E = E;

    return vertex;
}

void vertex_destroy(vertex_t *vertex) {
    if (vertex != NULL) {
        free(vertex->E);
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

    edge->source = v;
    edge->target = u;
    edge->w = w;

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
    graph_t *g_copy = graph_create();

    vertex_t *vertex;
    vertex_t *v_copy;
    for (int i = 0; i < graph->cap_v; i++) {
        vertex = graph->V[i];
        if (vertex == NULL)
            continue;

        v_copy = vertex_create(vertex->id);
        graph_insert_vertex(g_copy, v_copy);
    }

    edge_t *edge;
    edge_t *e_copy;
    for (int i = 0; i < graph->cap_e; i++) {
        edge = graph->E[i];
        if (edge == NULL)
            continue;

        e_copy = edge_create(g_copy->V[edge->source->id], g_copy->V[edge->target->id], edge->w);
        graph_insert_edge(g_copy, e_copy);
    }

    return g_copy;
}

path_t *path_create(int n) {
    path_t *path = malloc(sizeof(path_t));
    if (path == NULL)
        return NULL;

    path->n = n;
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

    return path;
}

void path_destroy(path_t *path) {
    if (path == NULL)
        return;

    free(path->d);
    free(path->V);
    free(path);
}

path_t *path_split(path_t *path, int i) {
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


    /* We then read the rest of the files and add all the remaining edges */
    for (int i = 1; i < n; i++) {
        for (int k = 0; k < i; k++) {
            fgetc(file);
            fgetc(file);
        }
        for (int j = i; j < n; j++) {
            c = fgetc(file);
            if (c != '0') {
                e = edge_create(graph->V[i], graph->V[j], c - '0');
                graph_insert_edge(graph, e);
            }
            fgetc(file);
        }
    }

    fclose(file);
    return graph;
}

/* Inserts the given vertex into the given graph, placing it at a position in then
 * graphs list of vertices equal to it's id. Does not check for overwriting */
int graph_insert_vertex(graph_t *graph, vertex_t *vertex) {
    while (vertex->id >= graph->cap_v) {
        graph->cap_v *= 2;
        vertex_t **new_V = realloc(graph->V, sizeof(vertex_t *) * graph->cap_v);
        if (new_V == NULL)
            return -1;

        for (int i = graph->n_v; i < graph->cap_v; i++) {
            new_V[i] = NULL;
        }

        graph->V = new_V;
    }
    graph->V[vertex->id] = vertex;
    graph->n_v++;

    return 0;
}

/* Removes the given edge from the given vertex's list of edges. Does not delete
 * anything */
int vertex_remove_edge(vertex_t *vertex, edge_t *edge) {
    for (int i = 0; i < vertex->cap; i++) {
        if (vertex->E[i] == edge) {
            vertex->E[i] = NULL;
            return 0;
        }
    }
    return -1;
}

/* Removes the given vertex from the given graph, deleting all edges, but not
 * the vertex itself */
int graph_remove_vertex(graph_t *graph, vertex_t *vertex) {
    if (graph->V[vertex->id] != vertex)
        return -1;

    graph->V[vertex->id] = NULL;

    int ret = 0;
    edge_t *edge;
    for (int i = 0; i < vertex->cap; i++) {
        edge = vertex->E[i];
        if (edge == NULL)
            continue;

        ret += vertex_remove_edge(edge->source, edge);
        ret += vertex_remove_edge(edge->target, edge);
        edge_destroy(edge);
    }

    return ret;
}

int vertex_insert_edge(vertex_t *v, edge_t *e) {
    if (v->n >= v->cap) {
        v->cap *= 2;
        edge_t **new_E = realloc(v->E, sizeof(edge_t *) * v->cap);
        if (new_E == NULL)
            return -1;

        v->E = new_E;
    }
    v->E[v->n] = e;
    v->n++;

    return 0;
}

int graph_insert_edge(graph_t *graph, edge_t *edge) {
    if (graph->n_e >= graph->cap_e) {
        graph->cap_e *= 2;
        edge_t **new_E = realloc(graph->E, sizeof(edge_t *) * graph->cap_e);
        if (new_E == NULL)
            return -1;

        graph->E = new_E;
    }
    graph->E[graph->n_e] = edge;
    graph->n_e++;

    int retval1 = vertex_insert_edge(edge->source, edge);
    int retval2 = vertex_insert_edge(edge->target, edge);
    if (retval1 || retval2)
        return -1;

    return 0;
}

int graph_remove_path(graph_t *graph, path_t *path) {
    int ret = 0;
    for (int i = 0; i < path->n; i++) {
        ret += graph_remove_vertex(graph, path->V[i]);
    }

    return ret;
}


void graph_components_helper(char *visited, int *belong, vertex_t *v, int graph) {
    visited[v->id] = 1;
    belong[v->id] = graph;

    vertex_t *u;
    for (int i = 0; i < v->n; i++) {
        u = edge_other_vertex(v, v->E[i]);
        if (!visited[u->id]) {
            graph_components_helper(visited, belong, u, graph);
        }
    }
}

graph_t **graph_components(graph_t *graph) {
    int n_v = graph->cap_v;
    int n_graphs = 0;
    int belong[n_v];
    char visited[n_v];
    memset(visited, 0, sizeof(int) * n_v);

    /* We determine the number of graphs and which graph each vertex belongs to */
    for (int i = 0; i < n_v; i++) {
        if (graph->V[i] == NULL)
            continue;

        if (!visited[i]) {
            graph_components_helper(visited, belong, graph->V[i], n_graphs);
            n_graphs++;
        }
    }

    /* We set up the array of graphs, and end with a NULL in order to indicate array length */
    graph_t **graphs = malloc(sizeof(graph_t *) * (n_graphs + 1));
    for (int i = 0; i < n_graphs; i++) {
        graphs[i] = graph_create();
    }
    graphs[n_graphs] = NULL;

    /* We insert the vertices into their respective graphs */
    for (int i = 0; i < n_v; i++) {
        if (graph->V[i] == NULL)
            continue;

        graph_insert_vertex(graphs[belong[i]], graph->V[i]);
    }
    /* We insert the edges into their respective graphs */
    edge_t *edge;
    for (int i = 0; i < graph->cap_e; i++) {
        edge = graph->E[i];
        if (edge == NULL)
            continue;

        graph_insert_edge(graphs[belong[edge->source->id]], edge);
    }

    /* We clean up the old graph */
    free(graph->V);
    free(graph->E);
    free(graph);

    return graphs;
}

void graph_print(graph_t *graph) {
    vertex_t *v;
    vertex_t *u;
    int w;

    printf("Graph:\n");
    // For each node in the graph print a line
    for (int i = 0; i < graph->cap_v; i++) {
        v = graph->V[i];
        if (v == NULL)
            continue;

        // For each node in the graph, print the weight if there is an edge
        for (int j = 0; j < graph->n_v; j++) {
            w = 0;

            // For each adjacent node, check if there is an edge
            for (int k = 0; k < v->n; k++) {
                u = edge_other_vertex(v, v->E[k]);
                if (u == graph->V[j]) {
                    w = v->E[k]->w;
                }
            }
            printf("%d ", w);
        }
        printf("\n");
    }
}
