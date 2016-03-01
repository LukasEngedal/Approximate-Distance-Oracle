#include "graph.h"

void vertex_init(vertex_t *vertex) {
    if (vertex == NULL) {
        printf("Error (vertex_init): Vertex is not initialized!\n");
        exit(EXIT_FAILURE);
    }
    vertex->prev = NULL;
    vertex->d = 0;
    vertex->n = 0;
    vertex->cap = 2;
    vertex->adj = malloc(sizeof(vertex_t *) * vertex->cap);
    assert(vertex->adj);
    vertex->w = (int *) malloc(sizeof(int) * vertex->cap);
    assert(vertex->w);
}

void graph_init(graph_t *graph) {
    graph->n_vertex = 0;
    graph->n_edge = 0;
    graph->cap = 8;
    graph->V = malloc(sizeof(vertex_t *) * graph->cap);
    assert(graph->V);
    
    /* for (int i = 0; i < graph->nv; i++) { */
    /*     vertex_t *vertex = malloc(sizeof(vertex_t)); */
    /*     assert(vertex); */
    /*     vertex_init(vertex); */
    /*     graph->V[i] = vertex; */
    /* } */
}

void vertex_destroy(vertex_t *vertex) {
    free(vertex->adj);
    free(vertex->w);
}

void graph_destroy(graph_t *graph) {
    for (int i = 0; i < graph->n_vertex; i++) {
        vertex_t *vertex = graph->V[i];
        vertex_destroy(vertex);
        free(vertex);
    }
    free(graph->V);
}

void graph_insert_vertex(graph_t *graph) {
    if (graph->n_vertex == graph->cap) {
        graph->cap *= 2;
        vertex_t **newV = realloc(graph->V, sizeof(void *) * graph->cap);
        assert(newV);
        graph->V = newV;
    }

    vertex_t *vertex = malloc(sizeof(vertex_t));
    assert(vertex);
    vertex_init(vertex);
    graph->V[graph->n_vertex] = vertex;
    graph->n_vertex++;
}

void vertex_insert_edge(vertex_t *v, vertex_t *u, int w) {
    if (v == NULL || u == NULL) {
        printf("Error (vertex_insert_edge): Vertex is not initialized!\n");
        exit(EXIT_FAILURE);
    }
    if (v->n == v->cap) {
        v->cap *= 2;
        vertex_t **new_adj = realloc(v->adj, sizeof(vertex_t *) * v->cap);
        assert(new_adj);
        int *new_w = (int *) realloc(v->w, sizeof(int) * v->cap);
        assert(new_w);
        v->adj = new_adj;
        v->w = new_w;
    }
    
    v->adj[v->n] = u;
    v->w[v->n] = w;
    v->n++;
}

void graph_insert_edge(graph_t *graph, int u, int v, int w) {
    if (w < 0) {
        printf("Error (graph_insert_edge): Negative weights are not allowed!\n");
        exit(EXIT_FAILURE);
    }
    if (u < graph->n_vertex && u > 0 && v < graph->n_vertex && v > 0) {
        vertex_insert_edge(graph->V[u], graph->V[v], w);
        vertex_insert_edge(graph->V[v], graph->V[u], w);
        graph->n_edge++;
    } else {
        printf("Error: No such vertex!\n");
        exit(EXIT_FAILURE);
    }
}

void graph_print(graph_t *graph) {
    printf("Graph:\n");
    // For each node in the graph print a line
    for (int i = 0; i < graph->n_vertex; i++) {
        vertex_t *vertex = graph->V[i];
        // For each node in the graph, print the weight if there is an edge
        for (int j = 0; j < graph->n_vertex; j++) {
            int w = 0;
            // For each adjacent node, check if there is an edge
            for (int k = 0; k < vertex->n; k++) {
                if (vertex->adj[k] == graph->V[j]) {
                    w = vertex->w[k];
                }
            }
            printf("%d ", w);
        }
        printf("\n");
    }
}
