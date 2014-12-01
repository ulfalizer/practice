#include "common.h"
#include "queue.h"

// Keep the length as a power of two to speed up wrapping.
#define INITIAL_BUF_LEN 16

void queue_init(Queue *queue) {
    queue->buf = emalloc(sizeof(void*)*INITIAL_BUF_LEN, "queue init");
    queue->start = 0;
    queue->end = 0;
    queue->buf_len = INITIAL_BUF_LEN;
}

void queue_free(Queue *queue) {
    free(queue->buf);
}

size_t queue_len(Queue *queue) {
    return (queue->end + queue->buf_len - queue->start) & (queue->buf_len - 1);
}

static void grow_and_add(Queue *queue, void *val) {
    void **new_buf;

    new_buf = emalloc(2*sizeof(void*)*queue->buf_len, "queue grow");
    // Copy the old contents to the beginning of the new buffer.
    if (queue->end < queue->start) {
        memcpy(new_buf,
          queue->buf + queue->start,
          sizeof(void*)*(queue->buf_len - queue->start));
        memcpy(new_buf + queue->buf_len - queue->start,
          queue->buf,
          sizeof(void*)*queue->end);
    }
    else
        memcpy(new_buf,
          queue->buf + queue->start,
          sizeof(void*)*(queue->end - queue->start));
    // Write the new element after the old contents.
    new_buf[queue->buf_len - 1] = val;
    // Free the old buffer.
    free(queue->buf);

    queue->buf = new_buf;
    queue->start = 0;
    queue->end = queue->buf_len;
    queue->buf_len *= 2;
}

void queue_add(Queue *queue, void *val) {
    size_t new_end = (queue->end + 1) & (queue->buf_len - 1);
    // Buffer full?
    if (new_end == queue->start)
        grow_and_add(queue, val);
    else {
        queue->buf[queue->end] = val;
        queue->end = new_end;
    }
}

void *queue_remove(Queue *queue) {
    void *res;
    if (queue->start == queue->end)
        fail("removing from empty queue");
    res = queue->buf[queue->start];
    queue->start = (queue->start + 1) & (queue->buf_len - 1);
    return res;
}
