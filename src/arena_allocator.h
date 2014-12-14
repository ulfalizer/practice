// Arena allocator for quickly allocating many objects and freeing them in a
// single operation. Handy for local and "scoped" allocations. Most allocations
// should be very fast.
//
// Keeps a linked list of buffers (with a header embedded at the start of each
// buffer), always allocating out of the head buffer if possible. If not enough
// space remains in the head buffer for an allocation, a new buffer is
// allocated and inserted at the head.
//
// A special case is an allocation too large to fit in a default-sized buffer.
// These use dedicated buffers (which are always precisely full) that are
// inserted just after the head element. Such allocations are assumed to be
// relatively rare.

// The values below are silly for testing purposes.

// Default chunk size. This includes the Chunk header.
#define DEFAULT_CHUNK_SIZE 128
// Memory returned by the allocator will be aligned to this many bytes.
#define ALIGN 32

typedef struct Chunk {
    union {
        struct {
            struct Chunk *next;
            char *start;
            char *end;
            alignas(ALIGN) char storage[];
        };
        struct {
            // Large chunks are always full and don't need 'start' and 'end'
            // pointers.
            struct Chunk *next_;
            alignas(ALIGN) char big_storage[];
        };
    };
} Chunk, *Arena;

_Static_assert((offsetof(Chunk, storage) & (ALIGN - 1)) == 0,
  "'storage' not properly aligned");
_Static_assert((offsetof(Chunk, big_storage) & (ALIGN - 1)) == 0,
  "'big_storage' not properly aligned");

void arena_init(Arena *arena);
// Frees all memory allocated from the arena.
void arena_free(Arena *arena);

// Allocates 'size' bytes from the arena.
void *arena_alloc(Arena *arena, size_t size);
