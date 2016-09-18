// A dynamically-resized queue implemented in a circular buffer

typedef struct Queue {
    void **buf;
    size_t buf_len;
    // Indices [start,end[ hold elements. The position just before 'start'
    // (modulo wrapping) is never used, which removes the need to distinguish
    // between empty and full buffers.
    size_t start, end;
} Queue;

void queue_init(Queue *queue);
void queue_free(Queue *queue);

// Returns the number of elements stored in 'queue'
size_t queue_len(Queue *queue);

// Appends a new element to 'queue'
void queue_add(Queue *queue, void *val);

// Removes and returns the first element in 'queue'. Must not be called with an
// empty queue.
void *queue_remove(Queue *queue);
