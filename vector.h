typedef struct Vector Vector;

Vector *vector_make();
void vector_free(Vector *vector);
size_t vector_len(Vector *vector);
void vector_add(Vector *vector, void *val);
void *vector_get(Vector *vector, size_t index);
