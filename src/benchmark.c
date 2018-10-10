#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "thorup04.h"
#include "graph.h"
#include "test_funcs.h"

#define BILLION 1000000000L

float average(float *array, int n) {
    int total = 0;

    for (int i = 0; i < n; i++) {
        total += array[i];
    }

    return total / (float)n;
}

float std_dev(float *array, int n, float avg) {
    float total = 0;

    for (int i = 0; i < n; i++) {
        total += powf((array[i] - avg), 2.0);
    }
    total = total / (float)n;

    return sqrt(total);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong number of arguments!\n");
        exit(EXIT_FAILURE);
    }

    int n_g = atoi(argv[1]);
    int n_v = 5000;
    int max = 10;
    float eps = 2;
    FILE *file = fopen("./data/5000_2.txt", "a");

    int x, y;
    if (n_v == 400) {
        x = 20;
        y = 20;
    } else if (n_v == 500) {
        x = 25;
        y = 20;
    } else if (n_v == 1000) {
        x = 30;
        y = 33;
    } else if (n_v == 2000) {
        x = 40;
        y = 50;
    } else if (n_v == 3000) {
        x = 60;
        y = 50;
    } else if (n_v == 4000) {
        x = 60;
        y = 66;
    } else if (n_v == 5000) {
        x = 71;
        y = 70;
    } else {
        printf("Number of vertices not defined\n");
        exit(EXIT_FAILURE);
    }
    n_v = x * y;

    uint64_t prepro_t[n_g];
    int prepro_s[n_g];
    float query_avg[n_g];
    float query_std[n_g];
    float diff_avg[n_g];
    float diff_std[n_g];
    float tmp_d_avg[n_v];
    float tmp_d_std[n_v];
    float tmp_t_avg[n_v];
    float tmp_t_std[n_v];
    float tmp_d[n_v];
    float tmp_t[n_v];

    uint64_t tdiff;
    struct timespec start, end;
    float avg, pd, dd, ddiff;
    int i, j, k, l, size;
    graph_t *g, *gc;
    preprocess_result_t *pr;
    dijkstra_result_t *dr;
    vertex_result_t *vr;
    covering_set_t *c;
    for (i = 0; i < n_g; i++) {
        printf("Run %d of %d\n", i+1, n_g);
        //printf("i %d\n", i);
        g = graph_generate(x, y, max);
        gc = graph_copy(g);

        /* Measure the time it took to preprocess the graph */
        clock_gettime(CLOCK_MONOTONIC, &start);
        pr = thorup_preprocess(gc, eps);
        clock_gettime(CLOCK_MONOTONIC, &end);
        tdiff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        //tdiff = (float)(end - start) / CLOCKS_PER_SEC;
        prepro_t[i] = tdiff;

        /* Measure the size of the distance oracle structure */
        size = 0;
        for (j = 0; j < n_v; j++) {
            vr = pr->V[j];
            for (k = 0; k < vr->n; k++) {
                c = vr->C[k];
                if (c == NULL)
                    continue;

                for (l = 0; l < c->n; l++) {
                    if (c->d[l] == -1)
                        continue;

                    size += 1;
                }
            }
        }
        prepro_s[i] = size;

        for (j = 0; j < n_v; j++) {
            //printf("j: %d\n", j);
            dr = dijkstra_sssp(g, j);

            for (k = 0; k < n_v; k++) {
                //printf("k %d\n", k);
                dd = (float)dijkstra_query(dr, k);

                /* Measure the time of querying */
                clock_gettime(CLOCK_MONOTONIC, &start);
                pd = (float)thorup_query(pr, j, k);
                clock_gettime(CLOCK_MONOTONIC, &end);
                tdiff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
                //printf("tdiff: %llu\n", (long long int)tdiff);
                tmp_t[k] = (float)tdiff;

                /* Measure the accuracy of the distance oracle */
                if (k == j) {
                    tmp_d[k] = 0;
                    continue;
                }
                //printf("dd: %d, pd: %d\n", dd, pd);
                ddiff = pd / dd;
                tmp_d[k] = ddiff;
                //printf("ddiff: %f\n", ddiff);
            }

            avg = average(tmp_t, n_v);
            //printf("avgt: %f\n", avg);
            tmp_t_avg[j] = avg;
            tmp_t_std[j] = std_dev(tmp_t, n_v, avg);

            avg = average(tmp_d, n_v);
            //printf("avgd: %f\n", avg);
            tmp_d_avg[j] = avg;
            tmp_d_std[j] = std_dev(tmp_d, n_v, avg);

            dijkstra_destroy(dr);
        }

        avg = average(tmp_t_avg, n_v);
        //printf("avgavgt: %f\n", avg);
        query_avg[i] = avg;
        query_std[i] = std_dev(tmp_t_std, n_v, 0);

        avg = average(tmp_d_avg, n_v);
        //printf("avgavgd: %f\n", avg);
        diff_avg[i] = avg;
        diff_std[i] = std_dev(tmp_d_std, n_v, 0);

        preprocess_result_destroy(pr);
        graph_destroy(g);

        fprintf(file, "%10llu\t%6d\t\t%4.2f\t\t%4.2f\t\t%6f\t%6f\n", (long long int)prepro_t[i], prepro_s[i],
                query_avg[i], query_std[i], diff_avg[i], diff_std[i]);
        fflush(file);
        sleep(1);
    }

    //fputs("prepro_t \tprepro_s \tquery_avg \tquery_std \tdiff_avg\tdiff_std\n", file);
    /* for (i = 0; i < n_g; i++) { */
    /*     fprintf(file, "%10llu\t%6d\t\t%4.2f\t\t%4.2f\t\t%6f\t%6f\n", (long long int)prepro_t[i], prepro_s[i], */
    /*             query_avg[i], query_std[i], diff_avg[i], diff_std[i]); */
    /* } */
    fclose(file);

    return 0;
}
