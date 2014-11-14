typedef struct Vector {
    void **buf;
    size_t buf_len;
    size_t len;
} Vector;

void vector_init(Vector *vector);
void vector_free(Vector *vector);
size_t vector_len(Vector *vector);
void vector_add(Vector *vector, void *val);
void *vector_get(Vector *vector, size_t index);
