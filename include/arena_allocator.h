// Arena allocator for quickly allocating many objects and freeing them in a
// single operation. Handy for local and "scoped" allocations. Most allocations
// should be very fast.
//
// Keeps a linked list of buffers called "chunks" (with a header embedded at
// the start of each chunk), always allocating out of the head chunk if
// possible. If not enough space remains in the head chunk for an allocation, a
// new chunk is allocated and inserted at the head (or a free chunk is reused
// -- see below).
//
// The implementation also supports cursors, which allow a position in the
// arena be recorded and later "wound back" to. All allocations done after a
// call to arena_get_cursor() can be efficiently freed with a call to
// arena_set_cursor() using the same cursor. Rewinding can be done either in
// "reuse mode" or in "free mode". In "reuse mode" discarded chunks will be
// kept around and reused if possible rather than free()d.
//
// Nesting of cursor operations is supported in the intuitive way.

// The internal structure with a set of allocated and freed reusable chunks is
// like in the following figure:
//
//         |--Allocated---|  |-----Free-----|
// first -> c1 -> c2 -> c3 -> c4 -> c5 -> c6
//                      ^
//                      |
//                     cur

// The values below are silly for testing purposes

// Default chunk size. This includes the Chunk header.
#define DEFAULT_CHUNK_SIZE 128
// Memory returned by the allocator will be aligned to this many bytes
#define ALIGN 32

typedef struct Chunk {
    struct Chunk *next;
    char *start;
    char *end;
    alignas(ALIGN) char storage[];
} Chunk;

static_assert((offsetof(Chunk, storage) & (ALIGN - 1)) == 0,
  "'storage' not properly aligned");

typedef struct Arena {
    Chunk *cur;
    Chunk *first;
} Arena;

typedef struct Arena_cursor {
    Chunk *chunk;
    char *start;
} Arena_cursor;

void arena_init(Arena *arena);
// Frees all memory allocated from the arena
void arena_free(Arena *arena);

// Allocates 'size' bytes from the arena
void *arena_alloc(Arena *arena, size_t size);

// Records the current position in the arena in 'cursor'. Later allocations can
// be freed with a call to arena_set_cursor() using the same cursor.
void arena_get_cursor(Arena *arena, Arena_cursor *cursor);

// Restores the position recorded in 'cursor', freeing allocations done after
// it. If 'reuse_chunks' is true, space from freed chunks is reused rather than
// immediately free()d.
void arena_set_cursor(Arena *arena, Arena_cursor *cursor, bool reuse_chunks);

// Like strdup(), allocating out of the arena
char *arena_strdup(Arena *arena, const char *s);

// Like strndup(), allocating out of the arena
char *arena_strndup(Arena *arena, const char *s, size_t n);
