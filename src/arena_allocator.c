#include "common.h"
#include "arena_allocator.h"

// Returns the next number greater than 'size' aligned modulo ALIGN.
static size_t g_aligned(size_t size) {
    return (size + ALIGN) & ~(ALIGN - 1);
}

void arena_init(Chunk **arena) {
    *arena = emalloc_align(DEFAULT_CHUNK_SIZE, ALIGN, "arena chunk init");
    (*arena)->next = NULL;
    (*arena)->start = (*arena)->storage;
    (*arena)->end = (char*)*arena + DEFAULT_CHUNK_SIZE;
}

void arena_free(Chunk **arena) {
    Chunk *next;
    for (; *arena != NULL; *arena = next) {
        next = (*arena)->next;
        free(*arena);
    }
}

void *arena_alloc(Chunk **arena, size_t size) {
    Chunk *chunk;

    if ((*arena)->start + size <= (*arena)->end) {
        // The allocation fits within the head chunk.
        void *res = (*arena)->start;
        (*arena)->start += g_aligned(size);

        return res;
    }

    // We need to allocate a new chunk.

    if (size <= DEFAULT_CHUNK_SIZE - sizeof(Chunk)) {
        // The allocation fits within a default-sized chunk.
        chunk = emalloc_align(DEFAULT_CHUNK_SIZE, ALIGN, "arena chunk small");
        chunk->next = *arena;
        chunk->start = chunk->storage + g_aligned(size);
        chunk->end = (char*)chunk + DEFAULT_CHUNK_SIZE;

        *arena = chunk;

        return chunk->storage;
    }
    // The allocation is too large for a default-sized chunk. Allocate a big
    // chunk.
    chunk = emalloc_align(offsetof(Chunk, big_storage) + size, ALIGN,
      "arena chunk large");
    // Insert the big chunk after the head chunk (so that we do not
    // accidentally attempt to allocate out of it).
    chunk->next = (*arena)->next;
    (*arena)->next = chunk;

    return chunk->big_storage;
}
