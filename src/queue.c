#include "queue.h"

queue_t *queue_create() {
    queue_t *queue = malloc(sizeof(queue_t));
    if (queue == NULL)
        return NULL;

    queue->size = 0;
    queue->start = NULL;

    return queue;
}

void queue_destroy(queue_t *queue) {
    if (queue != NULL)
        while (queue->size != 0) {
            queue_pop(queue);
        }
    free(queue);
}

int queue_push(queue_t *queue, void *item) {
    queue_item_t *q_item = malloc(sizeof(queue_item_t));
    if (q_item == NULL)
        return -1;

    q_item->item = item;
    q_item->next = NULL;
    queue->size++;

    queue_item_t *prev = queue->start;
    if (prev == NULL) {
        queue->start = q_item;
        return 0;
    }

    while (1) {
        if (prev->next == NULL)
            break;
        prev = prev->next;
    }
    prev->next = q_item;
    return 0;
}

void *queue_pop(queue_t *queue) {
    queue_item_t *q_item = queue->start;
    queue->start = q_item->next;
    queue->size--;

    void *item = q_item->item;
    free(q_item);

    return item;
}
