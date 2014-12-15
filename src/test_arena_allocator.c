// (Very) white-box testing.

#include "common.h"
#include "arena_allocator.h"

#define DEFAULT_STORAGE_SIZE (DEFAULT_CHUNK_SIZE - sizeof(Chunk))

static void test_one_byte_allocs() {
    Arena arena;
    arena_init(&arena);

    VERIFY(arena->end - arena->storage == DEFAULT_STORAGE_SIZE);

    // Allocate as many elements as will fit in the initial chunk, and
    // initialize each.
    for (int i = 0; i < DEFAULT_STORAGE_SIZE/ALIGN; ++i)
        *(char*)arena_alloc(&arena, 1) = 'a';
    // Should have only one chunk.
    VERIFY(arena->next == NULL);

    // Allocating one more element should allocate another chunk.
    *(char*)arena_alloc(&arena, 1) = 'z';
    VERIFY(arena->next->next == NULL);

    // Sanity check.
    VERIFY(arena != arena->next);

    VERIFY(arena->end - arena->storage == DEFAULT_STORAGE_SIZE);

    // Verify that the old chunk is filled with aligned 'a's.
    for (char *p = arena->next->storage; p < arena->next->end; p += ALIGN)
        VERIFY(*p == 'a');

    arena_free(&arena);
}

static void test_normal_allocs() {
    Arena arena;
    arena_init(&arena);
    char *s;

    // Allocate the largest block that will fit in a single normal-sized chunk.
    s = arena_alloc(&arena, DEFAULT_STORAGE_SIZE);
    // Should have only one chunk.
    VERIFY(arena->next == NULL);

    // See if we can initialize the storage without triggering a memory error.
    memset(s, 'a', DEFAULT_STORAGE_SIZE);

    // Allocate one more byte.
    *(char*)arena_alloc(&arena, 1) = 'x';
    // Should be two chunks now.
    VERIFY(arena->next->next == NULL);
    VERIFY(*(char*)arena->storage == 'x');
    VERIFY(*(char*)arena->next->storage == 'a');

    arena_free(&arena);
}

static void test_big_allocs() {
    Arena arena;
    arena_init(&arena);
    char *s;

    // Allocate a block just large enough to need a big chunk.
    s = arena_alloc(&arena, DEFAULT_STORAGE_SIZE + 1);
    // Should have two chunks (first small, second large).
    VERIFY(arena->next->next == NULL);

    // See if we can initialize the storage without triggering a memory error.
    memset(s, 'a', DEFAULT_STORAGE_SIZE + 1);
    // Should have written to the second chunk.
    VERIFY(*(char*)arena->next->big_storage == 'a');

    arena_free(&arena);
}

static void test_big_allocs_chain() {
    Arena arena;
    arena_init(&arena);
    Chunk *cur, *next;

    // Allocate three normal blocks.
    for (int i = 0; i < 3; ++i)
        arena_alloc(&arena, DEFAULT_STORAGE_SIZE);
    VERIFY(arena->next->next->next == NULL);
    cur = arena;
    next = arena->next;

    // Allocate a big block.
    arena_alloc(&arena, 2*DEFAULT_STORAGE_SIZE);

    // Big block should end up between 'cur' and 'next'.
    VERIFY(arena == cur);
    VERIFY(arena->next->next == next);

    arena_free(&arena);
}

static void test_align() {
    Arena arena;
    arena_init(&arena);
    for (int i = 1; i <= 3*DEFAULT_STORAGE_SIZE; ++i)
        VERIFY(((uintptr_t)arena_alloc(&arena, i) & (ALIGN - 1)) == 0);
    arena_free(&arena);
}

static void test_strdup() {
    Arena arena;
    char *s[4];

    arena_init(&arena);

    s[0] = arena_strdup(&arena, "");
    s[1] = arena_strdup(&arena, "a");
    s[2] = arena_strdup(&arena, "ab");
    s[3] = arena_strdup(&arena, "abc");
    VERIFY(strcmp(s[0], "") == 0);
    VERIFY(strcmp(s[1], "a") == 0);
    VERIFY(strcmp(s[2], "ab") == 0);
    VERIFY(strcmp(s[3], "abc") == 0);

    // Should be modifiable.
    for (int i = 0; i < ARRAY_LEN(s); ++i)
        *s[i] = 'x';

    arena_free(&arena);
}

static void test_strndup() {
    Arena arena;
    char *s[11];

    arena_init(&arena);

    s[0]  = arena_strndup(&arena, "", 0);
    s[1]  = arena_strndup(&arena, "", 1);
    s[2]  = arena_strndup(&arena, "a", 0);
    s[3]  = arena_strndup(&arena, "a", 1);
    s[4]  = arena_strndup(&arena, "a", 2);
    s[5]  = arena_strndup(&arena, "a", 3);
    s[6]  = arena_strndup(&arena, "ab", 1);
    s[7]  = arena_strndup(&arena, "ab", 2);
    s[8]  = arena_strndup(&arena, "ab", 3);
    s[9]  = arena_strndup(&arena, "foo bar", 3);
    s[10] = arena_strndup(&arena, "foo bar", 1000);
    VERIFY(strcmp(s[0], "") == 0);
    VERIFY(strcmp(s[1], "") == 0);
    VERIFY(strcmp(s[2], "") == 0);
    VERIFY(strcmp(s[3], "a") == 0);
    VERIFY(strcmp(s[4], "a") == 0);
    VERIFY(strcmp(s[5], "a") == 0);
    VERIFY(strcmp(s[6], "a") == 0);
    VERIFY(strcmp(s[7], "ab") == 0);
    VERIFY(strcmp(s[8], "ab") == 0);
    VERIFY(strcmp(s[9], "foo") == 0);
    VERIFY(strcmp(s[10], "foo bar") == 0);

    // Should be modifiable.
    for (int i = 0; i < ARRAY_LEN(s); ++i)
        *s[i] = 'x';

    arena_free(&arena);
}

void test_arena_allocator() {
    test_one_byte_allocs();
    test_normal_allocs();
    test_big_allocs();
    test_big_allocs_chain();
    test_align();
    test_strdup();
    test_strndup();
}
