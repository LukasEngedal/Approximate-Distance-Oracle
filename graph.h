#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct vertex_t {
    struct vertex_t *prev;    // The previous vertex in a shortest path
    int d;                    // The cost of getting from starting vertex s to this vertex
    int n;                    // Number of vertices in adj
    int cap;                  // Size of adj array
    struct vertex_t **adj;    // Pointer to vertices that share an edge with this one
    int *w;                   // Weight of said edges
} vertex_t;

typedef struct graph_t {
    int n_vertex;             // Number of vertices in the graph
    int n_edge;               // Number of edge in the graph
    int cap;                  // Size of the V array
    struct vertex_t **V;      // Array containing pointers to the vertices in the graph
} graph_t;

void vertex_init(vertex_t *vertex);

void graph_init(graph_t *graph);

void vertex_destroy(vertex_t *vertex);

void graph_destroy(graph_t *g);

void graph_insert_vertex(graph_t *graph);

void graph_insert_edge(graph_t *graph, int u, int v, int w);

void graph_print(graph_t *graph);

#endif /* GRAPH_H */
