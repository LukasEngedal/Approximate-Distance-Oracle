#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "fibheap.h"
#include "bheap.h"
#include "graph.h"

typedef struct dijkstra_result_t {
    int source;
    int n;
    int *valid;
    int *dist;
    vertex_t **prev;
} dijkstra_result_t;

void dijkstra_destroy(dijkstra_result_t *result);

dijkstra_result_t *dijkstra_sssp(graph_t *graph, int s);

int dijkstra_query(dijkstra_result_t *result, int t);

void dijkstra_print(dijkstra_result_t *result);

#endif //DIJKSTRA_H
