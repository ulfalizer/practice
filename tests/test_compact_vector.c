#include "common.h"
#include "compact_vector.h"

#define TEST_LEN 512

void test_compact_vector() {
    Compact_vector vector = compact_vector_make();

    for (int i = 0; i < TEST_LEN; ++i) {
        VERIFY(compact_vector_len(vector) == i);
        for (int j = 0; j < i; ++j)
            VERIFY(compact_vector_get(vector, j) == j);
        compact_vector_append(&vector, i);
    }

    for (int i = 0; i < TEST_LEN; ++i)
        compact_vector_set(vector, i, 2*i);

    for (int i = 0; i < TEST_LEN; ++i) {
        VERIFY(compact_vector_get(vector, i) == 2*i);
        VERIFY(compact_vector_storage(vector)[i] == 2*i);
    }

    compact_vector_free(vector);
}
