#include "tree.h"
#include "graph.h"
#include "dijkstra.h"
#include "thorup04.h"

#define PRINT_ALL 1
#define PRINT_GRAPH 0
#define PRINT_GRAPH_COMPONENT 0
#define PRINT_DIJKSTRA 0
#define PRINT_TREE 0
#define PRINT_LCA 0
#define PRINT_SP_TREE 0
#define PRINT_PATH 0
#define PRINT_RECUR2 0

int main() {
    int expected;
    int actual;
    int i;

    /*
     * Testing graphs
     */
    graph_t *graph = graph_from_file_M("graph_test.txt");

    expected = 13;
    actual = graph->n_v;
    if (actual != expected) {
        printf("Number of vertices in the graph from file is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 20;
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
     * Testing graph components
     */
    graph_t *graph_comp = graph_from_file_M("graph_component_test.txt");

    expected = 8;
    actual = graph_comp->n_v;
    if (actual != expected) {
        printf("Number of vertices in the comp graph is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 9;
    actual = graph_comp->n_e;
    if (actual != expected) {
        printf("Number of edges in comp graph is incorrect. Expected %d, got %d\n", expected, actual);
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

    expected = 4;
    actual = graphs[0]->n_v;
    if (actual != expected) {
        printf("Number of vertices in the component graph 1 is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }
    expected = 4;
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
    expected = 5;
    actual = graphs[1]->n_e;
    if (actual != expected) {
        printf("Number of edges in component graph 2 is incorrect. Expected %d, got %d\n", expected, actual);
        exit(EXIT_FAILURE);
    }


    /*
     * Testing Dijkstra
     */
    dijkstra_result_t *dijkstra_result = dijkstra_sssp(graph, 6);

    if (PRINT_DIJKSTRA || PRINT_ALL) {
        dijkstra_print(dijkstra_result);
        printf("\n");
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


    /*
     * Testing trees
     */
    int n_treenodes = 22;
    int parent[22] = {-1, 1, 2, 3, 4, 4, 3, 7, 8, 3, 1, 11, 1, 13, 14, 15, 15, 15, 18, 19, 19, 15};
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


    /*
     * Testing shortest path trees
     */
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


    /*
     * Testing paths
     */
    path_t *path = tree_root_path(sp_tree, (treenode_t *)graph->V[4]->node);

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

    if (PRINT_PATH || PRINT_ALL) {
        printf("Path 1 size: %d\n", path->n);
        printf("Path 1 legnth: %d\n", path->d[path->n - 1]);
        printf("Path 2 size: %d\n", path2->n);
        printf("Path 2 legnth: %d\n", path2->d[path2->n - 1]);
        printf("\n");
    }


    /*
     * Testing the second recursion of the main algorithm
     */
    graph_t *recur2_graph = graph_from_file_M("recursion2_test.txt");
    path_t *recur2_path = path_create(5);
    for (int i = 0; i < 5; i++) {
        recur2_path->V[i] = graph->V[i + 8];
        recur2_path->d[i] =i;
    }

    printf("n_v: %d\n", recur2_graph->n_v);

    float eps = 0.5;
    preprocess_result_t *recur2_result = preprocess_result_create(eps, recur2_graph->n_v);
    recursion2(recur2_graph, recur2_path, recur2_result);

    if (PRINT_RECUR2 || PRINT_ALL) {
        printf("N_v: %d\n", recur2_result->n_v);
        printf("N_p: %d\n", recur2_result->n_p);
    }

    /*
     * Cleaning Up
     */
    /* Graphs */
    graph_destroy(graph);

    /* Graph Components */
    for (int i = 0; i < n_graphs; i++) {
        graph_destroy(graphs[i]);
    }
    free(graphs);

    /* Dijkstra */
    dijkstra_destroy(dijkstra_result);

    /* Trees */
    tree_destroy(tree);

    /* LCA */
    lca_result_destroy(lca_result);

    /* Shortest Path Trees */
    tree_destroy(sp_tree);

    /* Paths */
    path_destroy(path);
    path_destroy(path2);

    return 0;
}
