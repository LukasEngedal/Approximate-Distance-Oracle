#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct queue_item_t {
    void *item;
    struct queue_item_t *next;
} queue_item_t;

typedef struct queue_t {
    int size;
    struct queue_item_t *start;
} queue_t;

queue_t *queue_create();

void queue_destroy(queue_t *queue);

int queue_push(queue_t *queue, void *item);

void *queue_pop(queue_t *queue);

#endif /* QUEUE_H */
