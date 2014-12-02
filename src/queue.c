#include "common.h"
#include "queue.h"

// Keep the length as a power of two to speed up wrapping. Using modulos
// instead makes repeated inserts and removes of 1000 elements ~4 times slower
// on my Core i7-2600K.
//
// Pre-calculating the mask (buf_len - 1) and updating it whenever the buffer
// size changes is excessive and actually makes things slightly slower on my
// system (perhaps due to extra memory instructions).
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

// Doubles the size of the buffer. Assumes the buffer is precisely full
// ('start' == 'end', here interpreted as "full").
//
// This grows the buffer as it becomes full -- not at the next insertion after
// that. Additional logic would allow the buffer to be precisely full.
static void grow(Queue *queue) {
    void **new_buf;

    assert(queue->start == queue->end);

    new_buf = emalloc(2*sizeof(void*)*queue->buf_len, "queue grow");

    // Copy from 'start' up to the end of the buffer.
    memcpy(new_buf,
      queue->buf + queue->start,
      sizeof(void*)*(queue->buf_len - queue->start));
    // Copy from the beginning of the buffer up to but not including
    // 'start' (== 'end').
    memcpy(new_buf + queue->buf_len - queue->start,
      queue->buf,
      sizeof(void*)*queue->start);
    // Free the old buffer.
    free(queue->buf);

    queue->buf = new_buf;
    queue->start = 0;
    queue->end = queue->buf_len;
    queue->buf_len *= 2;
}

void queue_add(Queue *queue, void *val) {
    queue->buf[queue->end] = val;
    queue->end = (queue->end + 1) & (queue->buf_len - 1);
    // Buffer full?
    if (queue->end == queue->start)
        grow(queue);
}

void *queue_remove(Queue *queue) {
    void *res;
    assert(queue->start != queue->end);
    res = queue->buf[queue->start];
    queue->start = (queue->start + 1) & (queue->buf_len - 1);
    return res;
}
