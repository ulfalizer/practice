#include "common.h"
#include "vector.h"

#define INITIAL_BUF_LEN 16

typedef struct Vector {
    void **buf;
    size_t buf_len;
    size_t len;
} Vector;

Vector *vector_make() {
    Vector *vector = malloc(sizeof(Vector));
    if (vector == NULL)
        err("malloc Vector");
    vector->buf = malloc(sizeof(void*)*INITIAL_BUF_LEN);
    if (vector->buf == NULL)
        err("malloc Vector buf");

    vector->buf_len = INITIAL_BUF_LEN;
    vector->len = 0;
    return vector;
}

void vector_free(Vector *vector) {
    free(vector->buf);
    free(vector);
}

size_t vector_len(Vector *vector) {
    return vector->len;
}

static void grow(Vector *vector) {
    void **new_buf;

    new_buf = malloc(2*sizeof(void*)*vector->buf_len);
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
