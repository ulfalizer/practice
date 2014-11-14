typedef struct Queue Queue;

Queue *queue_make();
void queue_free(Queue *queue);
size_t queue_len(Queue *queue);
void queue_add(Queue *queue, void *val);
void *queue_remove(Queue *queue);
