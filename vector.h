// A dynamically-resized array implementation.

typedef struct Vector {
    void **buf;
    size_t buf_len;
    size_t len;
} Vector;

void vector_init(Vector *vector);
void vector_free(Vector *vector);
// Returns the number of elements stored in 'vector'.
size_t vector_len(Vector *vector);
// Appends a new element to 'vector'.
void vector_add(Vector *vector, void *val);
// Gets the element at 'index' in 'vector'.
void *vector_get(Vector *vector, size_t index);
