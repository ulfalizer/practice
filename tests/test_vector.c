#include "common.h"
#include "vector.h"

#define TEST_LEN 512

void test_vector(void)
{
    Vector vector;

    vector_init(&vector);

    for (intptr_t i = 0; i < TEST_LEN; ++i) {
        VERIFY(vector_len(&vector) == i);
        for (intptr_t j = 0; j < i; ++j)
            VERIFY((intptr_t)vector_get(&vector, j) == j);
        vector_append(&vector, (void*)i);
    }

    for (intptr_t i = 0; i < TEST_LEN; ++i)
        vector_set(&vector, i, (void*)(2*i));

    for (intptr_t i = 0; i < TEST_LEN; ++i) {
        VERIFY((intptr_t)vector_get(&vector, i) == 2*i);
        VERIFY((intptr_t)vector_storage(&vector)[i] == 2*i);
    }

    vector_free(&vector);
}
