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

// Sets the element at index 'index' in 'vector' to 'val'. 'index' must be a
// valid index.
void vector_set(Vector *vector, size_t index, void *val);

// Returns the element at 'index' in 'vector'. The element must exist.
void *vector_get(Vector *vector, size_t index);

// Return a pointer to the beginning of the internal buffer. Easily abused,
// occasionally useful.
void **vector_storage(Vector *vector);
