#include "common.h"
#include "queue.h"

// Keep the length as a power of two to speed up wrapping
#define INITIAL_LEN 16

typedef struct Queue {
    void **storage;
    size_t len;
    size_t start, end;
} Queue;

Queue *queue_make() {
    Queue *queue = malloc(sizeof(Queue));
    if (queue == NULL)
        err("malloc Queue");
    queue->storage = malloc(sizeof(void*)*INITIAL_LEN);
    if (queue->storage == NULL)
        err("malloc Queue storage");

    queue->start = 0;
    queue->end = 0;
    queue->len = INITIAL_LEN;
    return queue;
}

void queue_free(Queue *queue) {
    free(queue->storage);
    free(queue);
}

size_t queue_len(Queue *queue) {
    return (queue->end + queue->len - queue->start) & (queue->len - 1);
}

static void grow_and_add(Queue *queue, void *val) {
    void **new_storage;

    new_storage = malloc(2*sizeof(void*)*queue->len);
    // Copy the old contents to the beginning of the new buffer
    if (new_storage == NULL)
        err("malloc Queue grow storage");
    if (queue->end < queue->start) {
        memcpy(new_storage,
          queue->storage + queue->start,
          sizeof(void*)*(queue->len - queue->start));
        memcpy(new_storage + queue->len - queue->start,
          queue->storage,
          sizeof(void*)*queue->end);
    }
    else
        memcpy(new_storage,
          queue->storage + queue->start,
          sizeof(void*)*(queue->end - queue->start));
    // Write the new element after the old contents
    new_storage[queue->len - 1] = val;
    // Free the old buffer
    free(queue->storage);

    queue->storage = new_storage;
    queue->start = 0;
    queue->end = queue->len;
    queue->len *= 2;
}

void queue_add(Queue *queue, void *val) {
    size_t new_end = (queue->end + 1) & (queue->len - 1);
    if (new_end == queue->start)
        grow_and_add(queue, val);
    else {
        queue->storage[queue->end] = val;
        queue->end = new_end;
    }
}

void *queue_remove(Queue *queue) {
    void *res;
    if (queue->start == queue->end)
        fail("removing from empty queue");
    res = queue->storage[queue->start];
    queue->start = (queue->start + 1) & (queue->len - 1);
    return res;
}
