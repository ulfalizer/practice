#define HEAP_MAX_SIZE 255

typedef struct Min_heap {
    // Leave one slot of extra room so we can always store an INT_MAX after the
    // heap's storage. This simplifies min_heap_get().
    int storage[HEAP_MAX_SIZE + 1];
    size_t len;
} Min_heap;

void min_heap_init(Min_heap *heap);
void min_heap_free(Min_heap *heap);
size_t min_heap_len(Min_heap *heap);
void min_heap_add(Min_heap *heap, int val);
int min_heap_remove(Min_heap *heap);
