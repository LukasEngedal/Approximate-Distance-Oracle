#include <math.h>

#include "dijkstra.h"

void initialize_single_source(graph_t *graph, int s) {
    vertex_t *vertex;
    for (int i = 0; i < graph->n_vertex; i++) {
        vertex = graph->V[i];
        vertex->d = INFINITY;
        vertex->prev = NULL;
    }
    graph->V[s]->d = 0;
}

void initialize_fibheap(graph_t *graph, fibheap_t *fibheap) {
    vertex_t *vertex;
    for (int i = 0; i < graph->n_vertex; i++) {
        vertex = graph->V[i];
        node_t *node = malloc(sizeof(node_t));
        node_init(node);
        node->owner = vertex;
        node->key = vertex->d;
        fibheap_insert(fibheap, node);
    }
}

void relax(vertex_t *u, vertex_t *v, int w) {
    if (v->d > u->d + w) {
        v->d = u->d + w;
        v->prev = u;
    }
}

void dijkstra(graph_t *graph, int s) {
    if (s >= graph->n_vertex) {
        printf("Error (dijkstra): No such vertex!\n");
        exit(EXIT_FAILURE);
    }
    fibheap_t *fibheap = malloc(sizeof(fibheap_t));
    fibheap_init(fibheap);
    initialize_fibheap(graph, fibheap);
    initialize_single_source(graph, s);
    fibheap_print(fibheap);
    graph_print(graph);
}
