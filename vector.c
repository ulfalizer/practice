#include "common.h"
#include "vector.h"

#define INITIAL_BUF_LEN 16

void vector_init(Vector *vector) {
    vector->buf = emalloc(sizeof(void*)*INITIAL_BUF_LEN, "vector init");
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
    void **new_buf;

    new_buf = emalloc(2*sizeof(void*)*vector->buf_len, "grow vector");
    memcpy(new_buf, vector->buf, sizeof(void*)*vector->buf_len);
    free(vector->buf);

    vector->buf = new_buf;
    vector->buf_len *= 2;
}

void vector_add(Vector *vector, void *val) {
    if (vector->len == vector->buf_len)
        grow(vector);
    vector->buf[vector->len++] = val;
}

void *vector_get(Vector *vector, size_t index) {
    return vector->buf[index];
}
