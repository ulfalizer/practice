typedef struct Min_heap Min_heap;

Min_heap *min_heap_make();
void min_heap_free(Min_heap *heap);
size_t min_heap_size(Min_heap *heap);
void min_heap_add(Min_heap *heap, int val);
int min_heap_get(Min_heap *heap);
