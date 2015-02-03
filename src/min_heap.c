#include "common.h"
#include "min_heap.h"

static size_t parent(size_t i) { return (i - 1)/2; }
static size_t left_child(size_t i) { return 2*i + 1; }
static size_t right_child(size_t i) { return 2*i + 2; }

void min_heap_init(Min_heap *heap) {
    heap->len = 0;
}

void min_heap_free(UNUSED Min_heap *heap) {}

size_t min_heap_len(Min_heap *heap) {
    return heap->len;
}

void min_heap_add(Min_heap *heap, int val) {
    size_t i;

    assert(heap->len < HEAP_MAX_SIZE);

    for (i = heap->len;
      i != 0 && val < heap->storage[parent(i)];
      i = parent(i))
        heap->storage[i] = heap->storage[parent(i)];
    heap->storage[i] = val;
    ++heap->len;
}

int min_heap_remove(Min_heap *heap) {
    size_t i;
    int res = heap->storage[0];
    int val = heap->storage[--heap->len];

    // Makes it safe to only check whether the left child is out of bounds. A
    // non-existent right child will never be selected as min_i.
    heap->storage[heap->len] = INT_MAX;
    i = 0;
    for (;;) {
        size_t min_i;

        if (left_child(i) >= heap->len)
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

    if (left_child(i) >= heap->len)
        return true;
    left_valid = valid_min_heap_rec(heap, left_child(i));
    if (right_child(i) >= heap->len)
        return left_valid;

    return left_valid && valid_min_heap_rec(heap, right_child(i));
}

bool valid_min_heap(Min_heap *heap) {
    return valid_min_heap_rec(heap, 0);
}
