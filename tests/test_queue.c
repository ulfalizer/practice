#include "common.h"
#include "queue.h"

void test_queue() {
    // Try to exercise corner cases related to growing the buffer.
    for (intptr_t n = 0; n < 512; ++n) {
        Queue queue;

        queue_init(&queue);

        VERIFY(queue_len(&queue) == 0);
        // Add 0, 1, 2, ..., n.
        for (intptr_t i = 0; i <= n; ++i)
            queue_add(&queue, (void*)i);
        VERIFY(queue_len(&queue) == n + 1);
        // Remove 0, 1, 2, ..., n-1.
        for (intptr_t i = 0; i < n; ++i)
            VERIFY((intptr_t)queue_remove(&queue) == i);
        VERIFY(queue_len(&queue) == 1);
        // Add 0, 1, 2, ..., n.
        for (intptr_t i = 0; i <= n; ++i)
            queue_add(&queue, (void*)i);
        VERIFY(queue_len(&queue) == n + 2);
        // Remove n.
        VERIFY((intptr_t)queue_remove(&queue) == n);
        VERIFY(queue_len(&queue) == n + 1);
        // Remove 0, 1, 2, ..., n.
        for (intptr_t i = 0; i <= n; ++i)
            VERIFY((intptr_t)queue_remove(&queue) == i);
        VERIFY(queue_len(&queue) == 0);

        queue_free(&queue);
    }
}
