#include "common.h"
#include "min_heap.h"

void test_min_heap() {
    Min_heap heap;

    min_heap_init(&heap);

    VERIFY(min_heap_len(&heap) == 0);
    min_heap_add(&heap, 8);
    min_heap_add(&heap, 2);
    min_heap_add(&heap, 1);
    min_heap_add(&heap, 73);
    min_heap_add(&heap, 7);
    VERIFY(min_heap_len(&heap) == 5);
    VERIFY(min_heap_remove(&heap) == 1);
    VERIFY(min_heap_remove(&heap) == 2);
    VERIFY(min_heap_remove(&heap) == 7);
    VERIFY(min_heap_remove(&heap) == 8);
    VERIFY(min_heap_remove(&heap) == 73);
    VERIFY(min_heap_len(&heap) == 0);

    min_heap_free(&heap);
}
