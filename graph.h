#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRAPH_INIT_V_SIZE (128)
#define GRAPH_INIT_E_SIZE (2 * GRAPH_INIT_V_SIZE)
#define VERTEX_INIT_E_SIZE (2)

typedef struct vertex_t {
    int id;                   // ID of the vertex
    int n;                    // Number of edges in adj
    int cap;                  // Size of adj array
    struct edge_t **E;        // Pointers to the edges
    void *node;               //The node representing the vertex in the heap or tree
} vertex_t;

typedef struct edge_t {
    int w;
    vertex_t *source;
    vertex_t *target;
} edge_t;

typedef struct graph_t {
    int n_v;                 // Number of vertices in the graph
    int n_e;                 // Number of edge in the graph
    int cap_v;               // Size of the V array
    int cap_e;               // Size of the E array
    struct vertex_t **V;     // Array containing pointers to the vertices in the graph
    struct edge_t **E;       // Array containing pointers to the edges in the graph
} graph_t;

typedef struct path_t {
    int n;
    vertex_t **V;
    int *d;
} path_t;

vertex_t *vertex_create(int id);
void vertex_destroy(vertex_t *vertex);

edge_t *edge_create(vertex_t *v, vertex_t *u, int w);
void edge_destroy(edge_t *edge);

graph_t *graph_create();
void graph_destroy(graph_t *graph);
graph_t *graph_copy(graph_t *graph);/* TODO:  */

path_t *path_create(int n);
void path_destroy(path_t *path);

/* Split the given path at vertex i, returning the part of the path from i to n */
path_t *path_split(path_t *path, int i);


/* Loads a graph from a file containing an adjacency matrix */
graph_t *graph_from_file_M(char *filename);


int graph_insert_vertex(graph_t *graph, vertex_t *vertex);
int graph_remove_vertex(graph_t *graph, vertex_t *vertex);
int graph_insert_edge(graph_t *graph, edge_t *edge);

int graph_remove_path(graph_t *graph, path_t *path);

/* Split a graph into a list of components in the form of new graphs */
graph_t **graph_components(graph_t *graph);


void graph_print(graph_t *graph);

#define edge_other_vertex(vertex, edge) ((edge->source == vertex) ? (edge->target) : (edge->source))

#endif /* GRAPH_H */
