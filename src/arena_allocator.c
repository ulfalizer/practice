#include "common.h"
#include "arena_allocator.h"

// Returns the next number greater than 'size' aligned modulo ALIGN.
static size_t g_aligned(size_t size) {
    return (size + ALIGN) & ~(ALIGN - 1);
}

// Returns the amount of data that fits in 'chunk', ignoring how much data it
// currently holds.
static size_t storage_size(Chunk *chunk) {
    return chunk->end - chunk->storage;
}

void arena_init(Arena *arena) {
    arena->cur = emalloc_align(DEFAULT_CHUNK_SIZE, ALIGN, "arena chunk init");
    arena->cur->next = NULL;
    arena->cur->start = arena->cur->storage;
    arena->cur->end = (char*)arena->cur + DEFAULT_CHUNK_SIZE;
    arena->first = arena->cur;
}

void arena_free(Arena *arena) {
    Chunk *next;

    assume(arena->first != NULL);
    for (Chunk *cur = arena->first; cur != NULL; cur = next) {
        next = cur->next;
        free(cur);
    }
}

void *arena_alloc(Arena *arena, size_t size) {
    // Assume 'start' + 'g_aligned(size)' won't overflow, which is reasonable
    // in most situations involving small- to medium-sized allocations (as high
    // addresses are usually reserved for the stack and kernel). More robust
    // size checking would compare against 'end' - 'start' and never let
    // 'start' move past 'end'.
    if (arena->cur->start + size <= arena->cur->end) {
        // The allocation fits within the head chunk.
        void *res = arena->cur->start;

        arena->cur->start += g_aligned(size);

        return res;
    }

    // We need a new chunk.

    if (arena->cur->next != NULL && size <= storage_size(arena->cur->next))
        // The allocation fits within the next chunk (allocated before
        // resetting the cursor). Reuse it.
        //
        // We could also do a search of all free chunks to try to find one that
        // can accommodate the request, but keep it simple. The allocator is
        // optimized for small allocations that can be satisfied by the first
        // free chunk.
        arena->cur = arena->cur->next;
    else {
        // Allocate a new chunk. We support large allocations (assumed to be
        // uncommon) via oversized chunks.
        //
        // We can't reorder chunks since that would break cursor semantics, so
        // oversized chunks will always incur some (likely negligible) overhead
        // at the next allocation to discover that the head chunk is full.
        size_t chunk_size = max(DEFAULT_CHUNK_SIZE, sizeof(Chunk) + size);
        Chunk *chunk = emalloc_align(chunk_size, ALIGN, "arena chunk");

        chunk->next = arena->cur->next;
        chunk->end = (char*)chunk + chunk_size;

        arena->cur->next = chunk;
        arena->cur = chunk;
    }
    arena->cur->start = arena->cur->storage + g_aligned(size);

    return arena->cur->storage;
}

void arena_get_cursor(Arena *arena, Arena_cursor *cursor) {
    cursor->chunk = arena->cur;
    cursor->start = arena->cur->start;
}

void arena_set_cursor(Arena *arena, Arena_cursor *cursor, bool reuse_chunks) {
    arena->cur = cursor->chunk;
    arena->cur->start = cursor->start;
    if (!reuse_chunks) {
        Chunk *c = arena->cur->next;
        Chunk *next;

        arena->cur->next = NULL;
        for (; c != NULL; c = next) {
            next = c->next;
            free(c);
        }
    }
}

char *arena_strdup(Arena *arena, const char *s) {
    size_t len = strlen(s) + 1;

    return memcpy(arena_alloc(arena, len), s, len);
}

char *arena_strndup(Arena *arena, const char *s, size_t n) {
    size_t len = strnlen(s, n);
    char *res = arena_alloc(arena, len + 1);

    res[len] = '\0';
    return memcpy(res, s, len);
}
