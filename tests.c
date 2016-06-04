#include <time.h>
#include <stdlib.h>
#include "tree.h"
#include "graph.h"
#include "dijkstra.h"
#include "thorup04.h"

#define PRINT_ALL 0
#define PRINT_GRAPH_GEN 0
#define PRINT_GRAPH 0
#define PRINT_GRAPH_COMPONENT 0
#define PRINT_DIJKSTRA 0
#define PRINT_TREE 0
#define PRINT_LCA 0
#define PRINT_SP_TREE 0
#define PRINT_PATH 0
#define PRINT_PST 1
#define PRINT_RECUR2 0
#define PRINT_QUERY 0
#define PRINT_MAIN 0

void test_graph(graph_t *graph) {
    int expected, actual;
    vertex_t *v;
    edge_t *e;
    edge_t *first;
    int n_v = 0;
    int n_e = 0;
    int nc, ncc;
    for (int i = 0; i < graph->cap_v; i++) {
        v = graph->V[i];
        if (v == NULL)
            continue;
        n_v++;

        /* Testing ID */
        expected = i;
        actual = v->id;
        if (actual != expected) {
            printf("Vertex in graph has wrong id. Expected: %d, Actual: %d\n", expected, actual);
            exit(EXIT_FAILURE);
        }

        /* Testing for 0 edges */
        first = v->edge;
        nc = 0;
        ncc = 0;
        if (first == NULL) {
            if (nc != v->n) {
                printf("Number of edges from vertex differs from n. Expected: %d, Actual: %d\n", v->n, nc);
                exit(EXIT_FAILURE);
            }
            continue;
        }
        nc++;
        ncc++;

        /* Counting the number of edges clockwise */
        e = first->c;
        while (e != first) {
            nc++;
            e = e->c;
        }

        /* Counting the number of edges counter-clockwise */
        e = first->cc;
        while (e != first) {
            ncc++;
            e = e->cc;
        }

        if (nc != ncc) {
            printf("Number of edges from vertex in c direction and cc direction differ. nc: %d, ncc: %d\n", nc, ncc);
            exit(EXIT_FAILURE);
        }
        if (nc != v->n) {
            printf("Number of edges from vertex differs from n. Expected: %d, Actual: %d\n", v->n, nc);
            exit(EXIT_FAILURE);
        }

        e = v->edge;
        for (int j = 0; j < nc; j++) {
            if (e->w < 0) {
                printf("Edge has negative weigth: %d.\n", e->w);
                exit(EXIT_FAILURE);
            }

            expected = i;
            actual = e->source->id;
            if (actual != expected) {
                printf("Edge source id is incorrect. Expected: %d, Actual: %d\n", expected, actual);
                exit(EXIT_FAILURE);
            }

            if (e->undirect == NULL) {
                printf("Edge does not have a undirect partner.\n");
                exit(EXIT_FAILURE);
            }

            expected = e->w;
            actual = e->undirect->w;
            if (actual != expected) {
                printf("Weight of edge undirect partner differs from weight of edge. Expected: %d, Actual: %d\n", expected, actual);
                exit(EXIT_FAILURE);
            }
            e = e->c;
        }

        n_e += nc;
    }

    expected = graph->n_v;
    actual = n_v;
    if (actual != expected) {
        printf("Number of actual vertices in the graph differs from n_v. Expected: %d, Actual: %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = graph->n_e;
    actual = n_e;
    if (actual != expected) {
        printf("Number of actual edges in the graph differs from n_e. Expected: %d, Actual: %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    n_e = 0;
    for (int i = 0; i < graph->cap_e; i++) {
        e = graph->E[i];
        if (e == NULL)
            continue;

        if (e->w < 0) {
            printf("Edge has weight %d\n", e->w);
        }

        n_e++;
    }

    expected = graph->n_e;
    actual = n_e;
    if (actual != expected) {
        printf("Number of actual edges in the graph differs from number in E. Expected: %d, Actual: %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
}

void test_triangle(graph_t *graph) {
    edge_t *e1, *e2, *e3, *e4;
    for (int i = 0; i < graph->cap_e; i++) {
        e1 = graph->E[i];
        if (e1 == NULL)
            continue;

        e2 = e1->undirect->cc;
        e3 = e2->undirect->cc;
        e4 = e3->undirect->cc;

        if (e1 != e4) {
            printf("Edges in graphs does not form a triangle in the cc direction!\n");
            printf("e1->s->id: %d, e1->t->id: %d\n", e1->source->id, e1->target->id);
            printf("e2->s->id: %d, e2->t->id: %d\n", e2->source->id, e2->target->id);
            printf("e3->s->id: %d, e3->t->id: %d\n", e3->source->id, e3->target->id);
            printf("e4->s->id: %d, e4->t->id: %d\n", e4->source->id, e4->target->id);
            exit(EXIT_FAILURE);
        }

        e2 = e1->undirect->c;
        e3 = e2->undirect->c;
        e4 = e3->undirect->c;

        if (e1 != e4) {
            printf("Edges in graphs does not form a triangle in the c direction!\n");
            printf("e1->s->id: %d, e1->t->id: %d\n", e1->source->id, e1->target->id);
            printf("e2->s->id: %d, e2->t->id: %d\n", e2->source->id, e2->target->id);
            printf("e3->s->id: %d, e3->t->id: %d\n", e3->source->id, e3->target->id);
            printf("e4->s->id: %d, e4->t->id: %d\n", e4->source->id, e4->target->id);
            exit(EXIT_FAILURE);
        }
    }
}

void test_tree(tree_t *tree) {
    int expected, actual;
    treenode_t *parent, *child;
    int size;
    int n = 0;

    queue_t *queue = queue_create();
    queue_push(queue, tree->root);

    while (queue->size > 0) {
        n++;
        parent = queue_pop(queue);
        child = parent->child;
        size = 1;
        while (child != NULL) {
            if (child->parent != parent) {
                printf("Treenode has wrong parent.\n");
                exit(EXIT_FAILURE);
            }

            expected = parent->lvl + 1;
            actual = child->lvl;
            if (actual != expected) {
                printf("Treenode level differs from parents level + 1. Expected: %d, Actual: %d\n", expected, actual);
                exit(EXIT_FAILURE);
            }

            size += child->size;
            queue_push(queue, child);
            child = child->sibling;
        }

        expected = parent->size;
        actual = size;
        if (actual != expected) {
            printf("Treenode actual size differs from size. Expected: %d, Actual: %d\n", expected, actual);
            exit(EXIT_FAILURE);
        }
    }

    expected = tree->size;
    actual = n;
    if (actual != expected) {
        printf("Number of actual nodes in tree differs from tree size. Expected: %d, Actual: %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    queue_destroy(queue);
}

void test_path(path_t *path) {
    int expected, actual;
    vertex_t *s, *t;
    edge_t *e, *c;

    expected = 0;
    actual = path->d[0];
    if (actual != expected) {
        printf("Path start vertex d wrong. Expected: %d, Actual: %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < path->n - 1; i++) {
        s = path->V[i];
        t = path->V[i+1];

        e = NULL;
        c = s->edge;
        for (int j = 0; j < s->n; j++) {
            if (c->target == t) {
                e = c;
                break;
            }
            c = c->c;
        }
        if (e == NULL) {
            printf("No edge found between adjacent path vertices\n");
            exit(EXIT_FAILURE);
        }
        if (!e->undirect || e->undirect->source != t || e->undirect->target != s) {
            printf("Edge->undirect between adjacent path vertices not found or invalid.\n");
            exit(EXIT_FAILURE);
        }

        expected = path->d[i] + e->w;
        actual = path->d[i+1];
        if (actual != expected) {
            printf("Path vertex d value different from previous d + w. Expected: %d, Actual: %d\n", expected, actual);
            exit(EXIT_FAILURE);
        }
    }
}

int main() {
    vertex_t *v;
    edge_t *e;
    treenode_t *tnode, *tnode2;
    int expected, actual, i, j, n;

    /*
     * Testing the graph generator
     */
    printf("Testing Graph Generator.\n");
    int x = 20;
    int y = 20;
    int max = 2;
    graph_t *gen_graph = graph_generate(x, y, max);

    if (PRINT_GRAPH_GEN || PRINT_ALL) {
        graph_print(gen_graph);
    }

    test_graph(gen_graph);
    test_triangle(gen_graph);

    expected = x * y;
    actual = gen_graph->n_v;
    if (actual != expected) {
        printf("Number of vertices in the generated graph is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    // Generates a lot of graphs
    if (0) {
        int ne;
        graph_t *graphh;
        for (int i = 2; i < 100; i++) {
            for (int j = 2; j < 100; j++) {
                graphh = graph_generate(i, j, 10);
                ne = i * j * 6 - 4 * i - 4 * j + 2;
                ne = ne;
                //printf("i: %d, j: %d, ne: %d, n_e: %d\n", i, j, ne, graphh->n_e);
                graph_destroy(graphh);
            }
        }
    }


    /*
     * Testing graph from file
     */
    printf("Testing Graph from File.\n");
    graph_t *graph = graph_from_file_M("graph_test.txt");

    test_graph(gen_graph);

    expected = 13;
    actual = graph->n_v;
    if (actual != expected) {
        printf("Number of vertices in the graph from file is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 40;
    actual = graph->n_e;
    if (actual != expected) {
        printf("Number of edges in the graph from file is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    if (PRINT_GRAPH || PRINT_ALL) {
        printf("Graph from file: \n");
        graph_print(graph);
        printf("\n");
    }


    /*
     * Testing graph copying
     */
    printf("Testing Graph Copying.\n");
    graph_t *g_copy = graph_copy(gen_graph);

    test_graph(g_copy);
    test_triangle(g_copy);

    expected = gen_graph->n_v;
    actual = g_copy->n_v;
    if (actual != expected) {
        printf("Number of vertices in the graph g_copy is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = gen_graph->n_e;
    actual = g_copy->n_e;
    if (actual != expected) {
        printf("Number of edges in the graph g_copy is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    vertex_t *vc;
    edge_t *ec;
    for (int i = 0; i < gen_graph->cap_v; i++) {
        v = gen_graph->V[i];
        vc = g_copy->V[i];
        if (v == NULL) {
            if (vc == NULL) {
                continue;
            } else {
                printf("Graph g_copy differs from original graph! (original vertex null, g_copy not)\n");
                exit(EXIT_FAILURE);
            }
        }
        if (vc == NULL) {
            printf("Graph g_copy differs from original graph! (original vertex not null, g_copy null) \n");
            exit(EXIT_FAILURE);
        }
        expected = v->id;
        actual = vc->id;
        if (actual != expected) {
            printf("Graph copy: vertex id's does not match. Expected: %d, Actual: %d\n", expected, actual);
            exit(EXIT_FAILURE);
        }
        expected = v->n;
        actual = vc->n;
        if (actual != expected) {
            printf("Graph copy: vertex number of edges does not match. Expected: %d, Actual: %d\n", expected, actual);
            exit(EXIT_FAILURE);
        }

        e = v->edge;
        ec = vc->edge;
        for (int j = 0; j < v->n; j++) {
            if (e == NULL) {
                if (ec == NULL) {
                    continue;
                } else {
                    printf("Graph g_copy differs from original graph! (original edge null, g_copy not)\n");
                    exit(EXIT_FAILURE);
                }
            }
            if (ec == NULL) {
                printf("Graph g_copy differs from original graph! (original edge not null, g_copy null) \n");
                exit(EXIT_FAILURE);
            }

            expected = e->source->id;
            actual = ec->source->id;
            if (actual != expected) {
                printf("Graph copy: edge source ID differs from copy. Expected: %d, Actual: %d\n", expected, actual);
                exit(EXIT_FAILURE);
            }
            expected = e->target->id;
            actual = ec->target->id;
            if (actual != expected) {
                printf("Graph copy: edge target ID differs from copy. Expected: %d, Actual: %d\n", expected, actual);
                exit(EXIT_FAILURE);
            }

            expected = e->undirect->source->id;
            actual = ec->undirect->source->id;
            if (actual != expected) {
                printf("Graph copy: undirect edge source ID differs from copy. Expected: %d, Actual: %d\n", expected, actual);
                exit(EXIT_FAILURE);
            }
            expected = e->undirect->target->id;
            actual = ec->undirect->target->id;
            if (actual != expected) {
                printf("Graph copy: undirect edge target ID differs from copy. Expected: %d, Actual: %d\n", expected, actual);
                exit(EXIT_FAILURE);
            }

            e = e->c;
            ec = ec->c;
        }
    }

    graph_destroy(g_copy);
    test_graph(gen_graph);
    test_triangle(gen_graph);


    /*
     * Testing graph components
     */
    printf("Testing Graph Components.\n");
    graph_t *graph_comp = graph_from_file_M("graph_component_test.txt");

    test_graph(graph_comp);

    expected = 8;
    actual = graph_comp->n_v;
    if (actual != expected) {
        printf("Number of vertices in the component graph is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 18;
    actual = graph_comp->n_e;
    if (actual != expected) {
        printf("Number of edges in component graph is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    graph_t **graphs = graph_components(graph_comp);
    int n_graphs = 0;
    while (graphs[n_graphs] != NULL) {
        n_graphs++;
    }

    if (PRINT_GRAPH_COMPONENT || PRINT_ALL) {
        printf("Number of graphs: %d\n", n_graphs);
        for (i = 0; i < n_graphs; i++) {
            printf("Component %d:\n", i);
            graph_print(graphs[i]);
        }
        printf("\n");
    }

    expected = 2;
    actual = n_graphs;
    if (actual != expected) {
        printf("Number of components in component graph is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    test_graph(graphs[0]);
    test_graph(graphs[1]);

    expected = 4;
    actual = graphs[0]->n_v;
    if (actual != expected) {
        printf("Number of vertices in the component graph 1 is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 8;
    actual = graphs[0]->n_e;
    if (actual != expected) {
        printf("Number of edges in component graph 1 is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 4;
    actual = graphs[1]->n_v;
    if (actual != expected) {
        printf("Number of vertices in the component graph 2 is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 10;
    actual = graphs[1]->n_e;
    if (actual != expected) {
        printf("Number of edges in component graph 2 is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }


    /*
     * Testing Binary Heaps
     */
    printf("Testing Binary Heaps.\n");
    bheap_t *bheap = bheap_create();

    n = 5;
    int keys[5] = {5, 3, 7, 11, 9};
    bnode_t *bnode;
    for (int i = 0; i < n; i++) {
        bnode = bnode_create();
        bheap_insert(bheap, bnode, keys[i]);
    }

    bnode_t *bnode2 = bheap_extract_min(bheap);
    expected = 3;
    actual = bnode2->key;
    if (actual != expected) {
        printf("Bheap min key is wrong. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    bnode_destroy(bnode2);

    bheap_decrease_key(bheap, bnode, 1);

    bnode2 = bheap_extract_min(bheap);
    expected = 1;
    actual = bnode2->key;
    if (actual != expected) {
        printf("Bheap min key after decrease key is wrong. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    bnode_destroy(bnode2);


    /*
     * Testing Dijkstra
     */
    printf("Testing Dijkstra.\n");
    test_graph(graph);
    dijkstra_result_t *dijkstra_result = dijkstra_sssp(graph, 6);
    if (dijkstra_result == NULL) {
        printf("Dijkstra result NULL\n");
        exit(EXIT_FAILURE);
    }

    expected = 12;
    actual = dijkstra_result->dist[4];
    if (actual != expected) {
        printf("Dijkstra distance to vertex 4 is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 1;
    actual = dijkstra_result->prev[4]->id;
    if (actual != expected) {
        printf("Dijkstra previous of vertex 4 is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    if (PRINT_DIJKSTRA || PRINT_ALL) {
        dijkstra_print(dijkstra_result);
        printf("\n");
    }

    n = gen_graph->n_v;
    int s = n / 2;
    dijkstra_result_t *gen_result = dijkstra_sssp(gen_graph, s);

    int t, dist_t, dist_s;
    dijkstra_result_t *gen_result2;
    for (i = 0; i < 4; i++) {
        t = n / 10 * 2 * i;
        dist_t = dijkstra_query(gen_result, t);

        gen_result2 = dijkstra_sssp(gen_graph, t);
        dist_s = dijkstra_query(gen_result2, s);

        if (dist_t != dist_s) {
            printf("Dijkstra on generated graph gives different result. S-T: %d, T-S: %d.\n", dist_t, dist_s);
            exit(EXIT_FAILURE);
        }
        dijkstra_destroy(gen_result2);
    }


    /*
     * Testing trees
     */
    printf("Testing Trees.\n");
    int n_treenodes = 22;
    int parent[22] = {-1, 1, 2, 3, 4, 4, 3, 7, 8, 3, 1, 11, 1, 13, 14, 15, 15, 15, 18, 19, 19, 15};
    int size[22] = {22, 9, 8, 3, 1, 1, 3, 2, 1, 1, 2, 1, 10, 9, 8, 1, 1, 4, 3, 1, 1, 1};
    int level[22] = {0, 1, 2, 3, 4, 4, 3, 4, 5, 3, 1, 2, 1, 2, 3, 4, 4, 4, 5, 6, 6, 4};
    treenode_t *treenodes[n_treenodes];

    treenode_t *root = treenode_create(0);
    treenodes[0] = root;
    tree_t *tree = tree_create(root);

    treenode_t *treenode;
    for (i = 1; i < n_treenodes; i++) {
        treenode = treenode_create(0);
        treenodes[i] = treenode;
        tree_insert_treenode(tree, treenodes[parent[i]-1], treenode);
    }

    tree_set_sizes(tree);
    tree_set_levels(tree);

    expected = n_treenodes;
    actual = tree->size;
    if (actual != expected) {
        printf("Tree size is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 0;
    actual = tree->root->id;
    if (actual != expected) {
        printf("Tree root id is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 8;
    actual = tree->root->child->child->child->sibling->child->child->id;
    if (actual != expected) {
        printf("Treenode id is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 21;
    actual = tree->root->child->sibling->sibling->child->child->child->sibling->sibling->sibling->id;
    if (actual != expected) {
        printf("Treenode id is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    for (i = 1; i < n_treenodes; i++) {
        treenode = treenodes[i];

        expected = size[i];
        actual = treenode->size;
        if (actual != expected) {
            printf("Tree node size is incorrect. Expected %d, got %d\n", expected, actual);
            exit(EXIT_FAILURE);
        }

        expected = level[i];
        actual = treenode->lvl;
        if (actual != expected) {
            printf("Tree node level is incorrect. Expected %d, got %d\n", expected, actual);
            exit(EXIT_FAILURE);
        }
    }

    test_tree(tree);


    /*
     * Testing breath first trees
     */
    printf("Testing Breath First Trees.\n");
    graph_t *tree_graph = graph_copy(gen_graph);
    test_graph(tree_graph);
    tree_t *bf_tree = breath_first_tree(tree_graph, tree_graph->n_v / 2);

    expected = tree_graph->n_v;
    actual = bf_tree->size;
    if (actual != expected) {
        printf("Size of breath first tree is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    expected = bf_tree->size;
    actual = bf_tree->root->size;
    if (actual != expected) {
        printf("Size of breath first tree differs from root size. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    test_tree(bf_tree);

    /*
     * Testing graph from tree
     */
    printf("Testing Graph from Tree.\n");
    graph_tree_reduce(tree_graph);
    test_graph(tree_graph);
    test_tree(bf_tree);

    expected = (bf_tree->size - 1) * 2;
    actual = tree_graph->n_e;
    if (actual != expected) {
        printf("Graph from tree has an incorrect number of edges. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < tree_graph->cap_v; i++) {
        v = tree_graph->V[i];
        if (v == NULL)
            continue;

        tnode = (treenode_t *)v->node;
        e = v->edge;
        for (j = 0; j < v->n; j++) {
            tnode2 = (treenode_t *)e->target->node;
            if (tnode->parent != tnode2 && tnode2->parent != tnode) {
                printf("Graph from tree node is neither parent nor child\n");
                exit(EXIT_FAILURE);
            }
            e = e->c;
        }
    }

    test_graph(tree_graph);

    /*
     * Testing shortest path trees
     */
    printf("Testing Shortest Path Trees.\n");
    tree_t *sp_tree = shortest_path_tree(graph, 6);

    expected = 13;
    actual = sp_tree->size;
    if (actual != expected) {
        printf("Size of shortest path tree is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 6;
    actual = ((vertex_t *)sp_tree->root->owner)->id;
    if (actual != expected) {
        printf("Root of shortest path tree is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 12;
    actual = ((treenode_t *)graph->V[4]->node)->value;
    if (actual != expected) {
        printf("Shortest path tree value of node is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 1;
    actual = ((vertex_t *)(((treenode_t *)graph->V[4]->node)->parent->owner))->id;
    if (actual != expected) {
        printf("Shortest path tree parent of node is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    test_tree(sp_tree);

    if (PRINT_SP_TREE || PRINT_ALL) {
        treenode_t *sp_treenode;
        printf("Shortest Path Tree:\n");
        printf("ID    PID\n");
        for (i = 0; i < sp_tree->size; i++) {
            sp_treenode = (treenode_t *)graph->V[i]->node;
            if (sp_treenode == sp_tree->root) {
                printf("%2d %6s\n", ((vertex_t *)sp_treenode->owner)->id, "Na");
            } else {
                printf("%2d %6d\n", ((vertex_t *)sp_treenode->owner)->id, ((vertex_t *)sp_treenode->parent->owner)->id);
            }
        }
        printf("\n");
    }

    i = rand() % gen_graph->cap_v;
    while (gen_graph->V[i] == NULL)
        i = rand() % gen_graph->cap_v;
    tree_t *sp_tree2 = shortest_path_tree(gen_graph, i);
    test_tree(sp_tree2);

    if (PRINT_SP_TREE || PRINT_ALL) {
        printf("Gen_Graph SP-tree:\n");
        tnode = sp_tree2->root;
        while (tnode != NULL) {
            printf("id: %3d, value: %3d\n", ((vertex_t *)tnode->owner)->id, tnode->value);
            tnode = tnode->child;
        }
    }


    /*
     * Testing LCA
     */
    lca_result_t *lca_result = lca_pre(tree);

    if (PRINT_LCA || PRINT_ALL) {
        printf("LCA:\n");
        printf("Length: %d\n", lca_result->length);
        printf("k:      %d\n", lca_result->k);

        printf("Node:      ");
        for (i = 0; i < n_treenodes; i++) {
            printf("%2d ", i);
        }
        printf("\n");

        printf("Preorder:  ");
        for (i = 0; i < n_treenodes; i++) {
            printf("%2d ", lca_result->preorder[i]);
        }
        printf("\n");

        printf("Size:      ");
        for (i = 0; i < n_treenodes; i++) {
            printf("%2d ", lca_result->size[i]);
        }
        printf("\n");

        printf("Inlabel:   ");
        for (i = 0; i < n_treenodes; i++) {
            printf("%2d ", lca_result->inlabel[i]);
        }
        printf("\n");

        printf("Ascendent: ");
        for (i = 0; i < n_treenodes; i++) {
            printf("%2d ", lca_result->ascendent[i]);
        }
        printf("\n");

        printf("Level:     ");
        for (i = 0; i < n_treenodes; i++) {
            printf("%2d ", lca_result->level[i]);
        }
        printf("\n");

        printf("ID: %d\n", lca(lca_result, treenodes[0], treenodes[15])->id);
        printf("\n");
    }

    treenode_t *lca1 = tree_lca_simple(treenodes[15], treenodes[19]);
    expected = 14;
    actual = lca1->id;
    if (actual != expected) {
        printf("LCA_simple result wrong. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    treenode_t *lca2 = tree_lca_simple(treenodes[4], treenodes[21]);
    expected = 0;
    actual = lca2->id;
    if (actual != expected) {
        printf("LCA_simple result wrong. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }


    /*
     * Testing paths
     */
    printf("Testing Paths.\n");
    path_t *path = tree_root_path((treenode_t *)graph->V[4]->node);

    expected = 13;
    actual = path->n;
    if (actual != expected) {
        printf("Size of path is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 12;
    actual = path->d[path->n - 1];
    if (actual != expected) {
        printf("Length of path is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    test_path(path);

    if (PRINT_PATH || PRINT_ALL) {
        printf("Paths:\n");
        printf("Path size: %d\n", path->n);
        printf("Path legnth: %d\n", path->d[path->n - 1]);
    }

    path_t *path2 = path_split(path, 7);

    expected = 8;
    actual = path->n;
    if (actual != expected) {
        printf("Size of path 1 is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 7;
    actual = path->d[path->n - 1];
    if (actual != expected) {
        printf("Length of path 1 is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 6;
    actual = path2->n;
    if (actual != expected) {
        printf("Size of path 2 is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 5;
    actual = path2->d[path2->n - 1];
    if (actual != expected) {
        printf("Length of path 2 is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    test_path(path);
    test_path(path2);

    if (PRINT_PATH || PRINT_ALL) {
        printf("Path 1 size: %d\n", path->n);
        printf("Path 1 legnth: %d\n", path->d[path->n - 1]);
        printf("Path 2 size: %d\n", path2->n);
        printf("Path 2 legnth: %d\n", path2->d[path2->n - 1]);
        printf("\n");
    }

    path_t *path2_f = path_flip(path2);
    test_path(path2_f);

    path_t *path_j = path_join(path, path2_f);
    test_path(path_j);

    expected = 13;
    actual = path_j->n;
    if (actual != expected) {
        printf("Size of path is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 12;
    actual = path_j->d[path_j->n - 1];
    if (actual != expected) {
        printf("Length of path is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    /*
     * Testing root paths
     */
    printf("Testing Tree Root Paths.\n");
    tnode = bf_tree->root;
    while (tnode->child != NULL) {
        tnode = tnode->child;
    }

    path_t *tpath = tree_root_path(tnode);

    expected = tnode->lvl + 1;
    actual = tpath->n;
    if (actual != expected) {
        printf("Length of root path differs from node level. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    tnode2 = (treenode_t *)tpath->V[tpath->n - 1]->node;
    if (tnode2 != tnode) {
        printf("Tree root path end node is incorrect.\n");
        exit(EXIT_FAILURE);
    }
    tnode2 = (treenode_t *)tpath->V[0]->node;
    if (tnode2 != bf_tree->root) {
        printf("Tree root path start node is not tree root.\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < tpath->n; i++) {
        tnode = (treenode_t *)(tpath->V[i]->node);

        expected = i;
        actual = tnode->lvl;
        if (actual != expected) {
            printf("Root path node position differs from node level. Expected %d, got %d\n", expected, actual);
            exit(EXIT_FAILURE);
        }
    }

    test_path(tpath);


    /*
     * Testing the separator theorem
     */
    printf("Testing the Separator Theorem.\n");

    srand(time(NULL));

    test_graph(gen_graph);
    test_triangle(gen_graph);

    graph_t *pst_graph = graph_copy(gen_graph);
    test_graph(pst_graph);
    test_triangle(pst_graph);

    path_t **separators = pst(pst_graph);
    if (PRINT_PST || PRINT_ALL) {
        for (i = 0; i < 3; i++) {
            printf("Size of pst_path: %d\n", separators[i]->n);
            //path_print(separators[i]);
        }
    }
    for (i = 0; i < 3; i++) {
        test_path(separators[i]);
    }

    vertex_t *vs[x*y];
    for (i = 0; i < x*y; i++) {
        vs[i] = NULL;
    }
    path_t *separator;
    for (i = 0; i < 3; i++) {
        separator = separators[i];

        for (j = 0; j < separator->n; j++) {
            v = separator->V[j];
            if (vs[v->id] == NULL) {
                vs[v->id] = v;
            }
        }
        path_destroy(separator);
    }
    for (i = 0; i < x*y; i++) {
        v = vs[i];
        if (v == NULL)
            continue;

        graph_remove_vertex(pst_graph, v);
        vertex_destroy(v);
    }
    free(separators);

    printf("Hertil?\n");

    test_graph(pst_graph);

    graph_t **pst_graphs = graph_components(pst_graph);
    int n_pst_graphs = 0;
    while (pst_graphs[n_pst_graphs] != NULL)
        n_pst_graphs++;

    if (PRINT_PST || PRINT_ALL) {
        printf("Number of graphs: %d\n", n_pst_graphs);
        for (i = 0; i < n_pst_graphs; i++) {
            printf("Size of graph %d: %d\n", i, pst_graphs[i]->n_v);
        }
    }

    graph_t *g;
    for (i = 0; i < n_pst_graphs; i++) {
        g = pst_graphs[i];
        test_graph(g);

        graph_triangulate(g);
        test_graph(g);
        test_triangle(g);
    }

    graph_destroy(pst_graphs[0]);
    for (i = 1; i < n_pst_graphs; i++) {
        g = pst_graphs[i];
        test_graph(g);
        test_triangle(g);
    }

    /*
     * Testing the second recursion of the main algorithm
     */
    graph_t *recur2_graph = graph_copy(gen_graph);
    path_t **recur2_paths = pst2(recur2_graph);
    path_t *recur2_path = recur2_paths[0];
    int n_v = recur2_graph->n_v;
    int n_p = recur2_path->n;

    test_graph(recur2_graph);
    test_path(recur2_path);
    //path_print(recur2_path);

    float eps = 0;
    preprocess_result_t *recur2_result = preprocess_result_create(eps, recur2_graph->n_v);
    recursion2(recur2_graph, recur2_path, recur2_result);

    expected = n_v;
    actual = recur2_result->n_v;
    if (actual != expected) {
        printf("Number of elements in preprocess_result_t is wrong. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 1;
    actual = recur2_result->n_p;
    if (actual != expected) {
        printf("Number of paths in preprocess_result_t is wrong. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }


    if (PRINT_RECUR2 || PRINT_ALL) {
        printf("N_v: %d\n", recur2_result->n_v);
        printf("N_p: %d\n", recur2_result->n_p);
    }

    /*
     * Testing Querying
     */
    printf("Testing Querying\n");
    s = x + 1;
    t = n_v - x - 2;
    covering_set_t *c_s = recur2_result->V[s]->C[0];
    covering_set_t *c_t = recur2_result->V[t]->C[0];
    int *p_d = recur2_result->P[0];

    expected = 1;
    actual = recur2_result->V[0]->n;
    if (actual != expected) {
        printf("Number of covering sets in vertex_result_t is wrong. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 0;
    actual = c_s->pid;
    if (actual != expected) {
        printf("ID of path in covering_set_t is wrong. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = n_p;
    actual = c_s->n;
    if (actual != expected) {
        printf("Number of path connections in covering_set_t is wrong. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }

    int dist = covering_set_dist(c_s, c_t, p_d);

    dijkstra_result_t *dijk_result_s = dijkstra_sssp(gen_graph, s);
    dijkstra_result_t *dijk_result_t = dijkstra_sssp(gen_graph, t);
    int dijkstra_dist_s = dijkstra_query(dijk_result_s, t);
    int dijkstra_dist_t = dijkstra_query(dijk_result_t, s);
    if (PRINT_QUERY || PRINT_ALL) {
        for (i = 0; i < n_p; i++) {
            printf("s->d[%2d] = %2d\n", i, c_s->d[i]);
        }
        for (i = 0; i < n_p; i++) {
            printf("s->d[%2d] = %2d\n", i, c_t->d[i]);
        }
        for (i = 0; i < n_p; i++) {
            printf("p->d[%2d] = %2d\n", i, p_d[i]);
        }

        printf("Covering set distance from s to t: %d\n", dist);
        printf("Dijkstra distance from s to t:     %d\n", dijkstra_dist_s);
        printf("Dijkstra distance from t to s:     %d\n", dijkstra_dist_t);
    }


    /*
     * Testing the main algorithm
     */
    //int run_main = 1;
    //if (run_main) {
        printf("Testing the Main Algorithm\n");
        graph_t *main_graph = graph_copy(gen_graph);
        eps = 0.5;
        preprocess_result_t *main_result = thorup_preprocess(main_graph, eps);

        s = rand() % gen_graph->n_v;
        dijkstra_result_t *dijk_result_main = dijkstra_sssp(gen_graph, s);

        int main_d, dijk_d, diff;
        if (PRINT_MAIN || PRINT_ALL) {
            for (i = 0; i < gen_graph->n_v; i++) {
                main_d = thorup_query(main_result, s, i);
                dijk_d = dijkstra_query(dijk_result_main, i);
                diff = main_d - dijk_d;
                printf("Oracle   distance from %3d to %3d: %3d\n", s, i, main_d);
                printf("Dijkstra distance from %3d to %3d: %3d\n", s, i, dijk_d);
                printf("Difference: %3d, t: %3f\n", diff, (float)diff / dijk_d);
            }
        }
        //}

    printf("Succes!\n");


    /*
     * Testing graph_remove_vertex
     */
    if (1) {
        int n = gen_graph->cap_v;
        for (int i = 0; i < n; i++) {
            v = gen_graph->V[i];
            if (v == NULL)
                continue;

            graph_remove_vertex(gen_graph, v);
            vertex_destroy(v);
        }
    }


    /*
     * Cleaning Up
     */
    /* Generated graph */
    graph_destroy(gen_graph);

    /* Graphs from file */
    graph_destroy(graph);

    /* Graph Components */
    for (int i = 0; i < n_graphs; i++) {
        graph_destroy(graphs[i]);
    }
    free(graphs);

    /* Bheap */
    bheap_destroy(bheap);

    /* Dijkstra */
    dijkstra_destroy(dijkstra_result);
    dijkstra_destroy(gen_result);

    /* Tree */
    tree_destroy(tree);

    /* Shortest Path Tree */
    tree_destroy(sp_tree);
    tree_destroy(sp_tree2);

    /* Breath First Tree */
    tree_destroy(bf_tree);
    graph_destroy(tree_graph);

    /* LCA */
    lca_result_destroy(lca_result);

    /* Paths */
    path_destroy(path);
    path_destroy(path2);
    path_destroy(path2_f);
    path_destroy(path_j);

    /* Root Paths */
    path_destroy(tpath);

    /* Separator Theorem */
    for (i = 1; i < n_pst_graphs; i++) {
        graph_destroy(pst_graphs[i]);
    }
    free(pst_graphs);

    /* Recursion 2 */
    //graph_destroy(recur2_graph);
    //path_destroy(recur2_path);
    free(recur2_paths);
    preprocess_result_destroy(recur2_result);
    dijkstra_destroy(dijk_result_s);
    dijkstra_destroy(dijk_result_t);

    /* Main Algorithm */
    //graph_destroy(main_graph);
    preprocess_result_destroy(main_result);
    dijkstra_destroy(dijk_result_main);


    return 0;
}
