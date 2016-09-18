// A dynamically-resized array with the header and buffer stored together.
//
// Provides zero-overhead set and get operations (provided compact_vector_set()
// and compact_vector_get() are inlined, e.g. via link-time optimization) by
// representing a Compact_vector as a direct pointer to its buffer. The
// downside is a slightly inconsistent interface (and a slightly trickier
// implementation).

// Efficiently copyable
typedef int *Compact_vector;

typedef struct Compact_vector_data {
    int *cur;
    int *end;
    int buf[];
} Compact_vector_data;

Compact_vector compact_vector_make(void);
void compact_vector_free(Compact_vector vector);

// Returns the number of elements stored in 'vector'
size_t compact_vector_len(Compact_vector vector);

// Appends 'val' to 'vector', growing it if necessary
void compact_vector_append(Compact_vector *vector, int val);

// Sets the element at index 'index' in 'vector' to 'val'. 'index' must be a
// valid index.
void compact_vector_set(Compact_vector vector, size_t index, int val);

// Returns the element at 'index' in 'vector'. 'index' must be a valid index.
int compact_vector_get(Compact_vector vector, size_t index);

// Return a pointer to the beginning of the internal buffer. Easily abused,
// occasionally useful.
int *compact_vector_storage(Compact_vector vector);
