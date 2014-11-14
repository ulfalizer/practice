typedef struct Queue {
    void **buf;
    size_t buf_len;
    size_t start, end;
} Queue;

void queue_init(Queue *queue);
Queue *queue_make();
void queue_free(Queue *queue);
size_t queue_len(Queue *queue);
void queue_add(Queue *queue, void *val);
void *queue_remove(Queue *queue);
