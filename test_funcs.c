#include "test_funcs.h"
#include "graph.h"
#include "tree.h"

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
