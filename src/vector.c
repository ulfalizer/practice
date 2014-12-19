#include "common.h"
#include "vector.h"

#define INITIAL_BUF_LEN 16

void vector_init(Vector *vector) {
    vector->buf = emalloc(sizeof(*vector->buf)*INITIAL_BUF_LEN, "vector init");
    vector->buf_len = INITIAL_BUF_LEN;
    vector->len = 0;
}

void vector_free(Vector *vector) {
    free(vector->buf);
}

size_t vector_len(Vector *vector) {
    return vector->len;
}

static void grow(Vector *vector) {
    vector->buf_len *= 2;
    vector->buf = erealloc(vector->buf, sizeof(*vector->buf)*vector->buf_len,
                           "vector grow");
}

void vector_add(Vector *vector, void *val) {
    if (vector->len == vector->buf_len)
        grow(vector);
    vector->buf[vector->len++] = val;
}

void *vector_get(Vector *vector, size_t index) {
    return vector->buf[index];
}
