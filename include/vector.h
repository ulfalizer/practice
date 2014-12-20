// A dynamically-resized array.

typedef struct Vector {
    void **buf;
    size_t buf_len;
    size_t len;
} Vector;

void vector_init(Vector *vector);
void vector_free(Vector *vector);

// Returns the number of elements stored in 'vector'.
size_t vector_len(Vector *vector);

// Appends 'val' to 'vector', growing it if necessary.
void vector_append(Vector *vector, void *val);

// Returns the element at 'index' in 'vector'. The element must exist.
void *vector_get(Vector *vector, size_t index);
