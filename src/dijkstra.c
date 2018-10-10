#include <math.h>

#include "dijkstra.h"

#define HEAPTYPE 1          // 0 for fibheap, 1 for binary heap

dijkstra_result_t *dijkstra_create(int n, int s) {
    dijkstra_result_t *result = malloc(sizeof(dijkstra_result_t));
    if (result == NULL)
        return NULL;

    int *valid = malloc(sizeof(int) * n);
    if (valid == NULL) {
        free(result);
        return NULL;
    }

    int *dist = malloc(sizeof(int) * n);
    if (dist == NULL) {
        free(valid);
        free(result);
        return NULL;
    }

    vertex_t **prev = malloc(sizeof(vertex_t *) * n);
    if (prev == NULL) {
        free(valid);
        free(dist);
        free(result);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        valid[i] = 0;
        dist[i] = 1<<30;
        prev[i] = NULL;
    }

    dist[s] = 0;
    result->valid = valid;
    result->dist = dist;
    result->prev = prev;
    result->source = s;
    result->n = n;

    return result;
}

void dijkstra_destroy(dijkstra_result_t *result) {
    free(result->valid);
    free(result->dist);
    free(result->prev);
    free(result);
}

void dijkstra_fibheap(graph_t *graph, dijkstra_result_t *result) {
    vertex_t *vertex;
    vertex_t *neighbor;
    edge_t *edge;
    node_t *node;
    int new_dist;

    fibheap_t *fibheap = (fibheap_t *)malloc(sizeof(fibheap_t));
    fibheap_init(fibheap);
    for (int i = 0; i < graph->n_v; i++) {
        vertex = graph->V[i];
        node = (node_t *)malloc(sizeof(node_t));
        node_init(node);
        node->key = result->dist[i];
        node->owner = (void *)vertex;
        vertex->node = (void *)node;
        fibheap_insert(fibheap, node);
    }

    while (!fibheap_empty(fibheap)) {
        node = fibheap_extract_min(fibheap);
        vertex = (vertex_t *)node->owner;
        edge = vertex->edge;

        for (int i = 0; i < vertex->n; i++) {
            neighbor = edge->target;
            new_dist = result->dist[vertex->id] + edge->w;
            if (result->dist[neighbor->id] > new_dist) {
                result->dist[neighbor->id] = new_dist;
                result->prev[neighbor->id] = vertex;
                fibheap_decrease_key(fibheap, neighbor->node, new_dist);
            }
            edge = edge->c;
        }
        free(node);
    }
    free(fibheap);
}

int dijkstra_bheap(graph_t *graph, dijkstra_result_t *result) {
    vertex_t *vertex;
    vertex_t *neighbor;
    edge_t *edge;
    bnode_t *bnode;
    int new_dist;

    bheap_t *bheap = bheap_create();
    if (bheap == NULL)
        return -1;

    for (int i = 0; i < graph->cap_v; i++) {
        vertex = graph->V[i];
        if (vertex == NULL)
            continue;
        result->valid[i] = 1;

        bnode = bnode_create();
        if (bnode == NULL)
            return -1;

        bnode->owner = (void *)vertex;
        vertex->node = (void *)bnode;
        if (bheap_insert(bheap, bnode, result->dist[i]))
            return -1;
    }

    while (!bheap_empty(bheap)) {
        bnode = bheap_extract_min(bheap);
        if (bnode == NULL)
            return -1;

        vertex = (vertex_t *)bnode->owner;
        edge = vertex->edge;

        for (int i = 0; i < vertex->n; i++) {
            neighbor = edge->target;
            new_dist = result->dist[vertex->id] + edge->w;
            //if (new_dist < 0 || new_dist > 1<<30)
            //    printf("dijkstra_bheap: extreme dist: %d\n", new_dist);
            if (result->dist[neighbor->id] > new_dist) {
                result->dist[neighbor->id] = new_dist;
                result->prev[neighbor->id] = vertex;
                if (bheap_decrease_key(bheap, (bnode_t *)neighbor->node, new_dist))
                    return -1;
            }
            edge = edge->c;
        }
        bnode_destroy(bnode);
    }
    bheap_destroy(bheap);

    return 0;
}

dijkstra_result_t *dijkstra_sssp(graph_t *graph, int s) {
    if (s >= graph->cap_v || s < 0 || graph->V[s] == NULL)
        return NULL;

    dijkstra_result_t *result = dijkstra_create(graph->cap_v, s);

    if (HEAPTYPE) {
        if (dijkstra_bheap(graph, result))
            return NULL;
    } else {
        dijkstra_fibheap(graph, result);
    }

    return result;
}

int dijkstra_query(dijkstra_result_t *result, int t) {
    if (result->valid[t] == 0)
        return -1;
    return result->dist[t];
}

void dijkstra_print(dijkstra_result_t *result) {
    printf("Dijkstra:\n");
    printf("Vertex    dist    prev\n");
    for (int i = 0; i < result->n; i++) {
        if (i == result->source) {
            printf("%6d %7d %7s\n", i, result->dist[i], "Na");
        } else {
            if (result->valid[i])
                printf("%6d %7d %7d\n", i, result->dist[i], result->prev[i]->id);
        }
    }
}
