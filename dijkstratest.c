#include "dijkstra.h"

#define NV 6
#define NE 10

int main() {
    graph_t *graph = malloc(sizeof(graph_t));
    assert(graph);
    graph_init(graph);
    
    for (int i = 0; i < NV; i++) {
        graph_insert_vertex(graph);
    }
    
    int v[NE] = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    int u[NE] = {1, 2, 2, 3, 4, 3, 4, 4, 5, 5};
    int w[NE] = {2, 4, 1, 6, 9, 4, 9, 3, 5, 1};
    for (int i = 0; i < NE; i++) {
        graph_insert_edge(graph, v[i], u[i], w[i]);
    }
    
    //dijkstra(graph, 0);

    return 0;
}
