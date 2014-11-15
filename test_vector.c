#include "common.h"
#include "vector.h"

void test_vector() {
    Vector vector;

    vector_init(&vector);
    for (intptr_t i = 0; i < 512; ++i) {
        VERIFY(vector_len(&vector) == i);
        for (intptr_t j = 0; j < i; ++j)
            VERIFY((intptr_t)vector_get(&vector, j) == j);
        vector_add(&vector, (void*)i);
    }
    vector_free(&vector);
}
