#include "common.h"
#include "min_heap.h"

#define HEAP_MAX_SIZE 255

typedef struct Min_heap {
    // Leave one slot of extra room so we can always store an INT_MAX after the
    // heap's storage. This simplifies min_heap_get().
    int storage[HEAP_MAX_SIZE + 1];
    size_t size;
} Min_heap;

static size_t parent(size_t i) { return (i - 1)/2; }
static size_t left_child(size_t i) { return 2*i + 1; }
static size_t right_child(size_t i) { return 2*i + 2; }

Min_heap *min_heap_make() {
    Min_heap *heap = malloc(sizeof(Min_heap));
    if (heap == NULL)
        err("malloc heap");
    heap->size = 0;
    return heap;
}

void min_heap_free(Min_heap *heap) {
    free(heap);
}

size_t min_heap_size(Min_heap *heap) {
    return heap->size;
}

void min_heap_add(Min_heap *heap, int val) {
    size_t i;

    if (heap->size == HEAP_MAX_SIZE)
        fail("heap full");

    for (i = heap->size;
      i != 0 && val < heap->storage[parent(i)];
      i = parent(i))
        heap->storage[i] = heap->storage[parent(i)];
    heap->storage[i] = val;
    ++heap->size;
}

int min_heap_get(Min_heap *heap) {
    size_t i;
    int res, val;
    if (heap->size == 0)
        fail("heap empty");
    res = heap->storage[0];
    val = heap->storage[--heap->size];
    // Makes it safe to only check whether the left child is out of bounds. A
    // non-existent right child will never be selected as min_i.
    heap->storage[heap->size] = INT_MAX;
    i = 0;
    for (;;) {
        size_t min_i;
        if (left_child(i) >= heap->size)
            break;
        min_i =
          (heap->storage[left_child(i)] < heap->storage[right_child(i)]) ?
            left_child(i) : right_child(i);
        if (val <= heap->storage[min_i])
            break;
        heap->storage[i] = heap->storage[min_i];
        i = min_i;
    }
    heap->storage[i] = val;
    return res;
}

static bool valid_min_heap_rec(Min_heap *heap, size_t i) {
    bool left_valid;
    if (left_child(i) >= heap->size)
        return true;
    left_valid = valid_min_heap_rec(heap, left_child(i));
    if (right_child(i) >= heap->size)
        return left_valid;
    return left_valid && valid_min_heap_rec(heap, right_child(i));
}

bool valid_min_heap(Min_heap *heap) {
    return valid_min_heap_rec(heap, 0);
}
