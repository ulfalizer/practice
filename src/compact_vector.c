#include "common.h"
#include "compact_vector.h"

#define INITIAL_BUF_LEN 32

#define HEADER_SIZE offsetof(Compact_vector_data, buf)

// Turns a pointer to 'buf' into a pointer to the enclosing
// Compact_vector_data.
//
// Using container_of() would be neater, but it currently runs into problems
// due to 'buf' being a flexible array member.
#define DATA(ptr) ((Compact_vector_data*)((char*)ptr - HEADER_SIZE))

Compact_vector compact_vector_make() {
    Compact_vector_data *vec_data =
      emalloc(HEADER_SIZE + sizeof(*vec_data->buf)*INITIAL_BUF_LEN,
              "compact vector init");

    vec_data->cur = vec_data->buf;
    vec_data->end = vec_data->buf + INITIAL_BUF_LEN;

    return vec_data->buf;
}

void compact_vector_free(Compact_vector vector) {
    free(DATA(vector));
}

size_t compact_vector_len(Compact_vector vector) {
    return DATA(vector)->cur - vector;
}

// Doubles the size of the buffer (excluding the header). Assumes the buffer is
// precisely full.
static Compact_vector_data *grow(Compact_vector_data *vec_data) {
    // Old length in int's.
    size_t old_buf_len = vec_data->end - vec_data->buf;

    vec_data = erealloc(vec_data,
                        HEADER_SIZE + 2*sizeof(*vec_data->buf)*old_buf_len,
                        "compact vector grow");
    vec_data->cur = vec_data->buf + old_buf_len;
    vec_data->end = vec_data->buf + 2*old_buf_len;

    return vec_data;
}

void compact_vector_append(Compact_vector *vector, int val) {
    Compact_vector_data *vec_data = DATA(*vector);

    if (vec_data->cur == vec_data->end) {
        vec_data = grow(vec_data);
        *vector = vec_data->buf;
    }
    *vec_data->cur++ = val;
}

void compact_vector_set(Compact_vector vector, size_t index, int val) {
    vector[index] = val;
}

int compact_vector_get(Compact_vector vector, size_t index) {
    return vector[index];
}

int *compact_vector_storage(Compact_vector vector) {
    return vector;
}
