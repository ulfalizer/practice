// (Very) white-box testing.

#include "common.h"
#include "arena_allocator.h"

#define DEFAULT_STORAGE_SIZE (DEFAULT_CHUNK_SIZE - sizeof(Chunk))

static unsigned n_chunks(Arena *arena) {
    Chunk *c;
    unsigned n;

    for (c = arena->first, n = 0; c != NULL; c = c->next, ++n);
    return n;
}

static size_t chunk_size(Chunk *chunk) {
    return chunk->end - chunk->storage;
}

static void test_empty() {
    Arena arena;

    arena_init(&arena);
    VERIFY(n_chunks(&arena) == 1);
    VERIFY(chunk_size(arena.first) == DEFAULT_STORAGE_SIZE);
    VERIFY(arena.cur == arena.first);
    arena_free(&arena);
}

static void test_one_byte_allocs() {
    Arena arena;

    arena_init(&arena);

    // Allocate as many elements as will fit in the initial chunk and
    // initialize each element.
    for (size_t i = 0; i < DEFAULT_STORAGE_SIZE/ALIGN; ++i)
        *(char*)arena_alloc(&arena, 1) = i;
    VERIFY(n_chunks(&arena) == 1);

    // Allocating one more element should allocate another chunk.
    *(char*)arena_alloc(&arena, 1) = 'a';
    VERIFY(n_chunks(&arena) == 2);

    VERIFY(chunk_size(arena.cur) == DEFAULT_STORAGE_SIZE);
    VERIFY(arena.cur != arena.first);

    // Verify contents.
    for (size_t i = 0; i < DEFAULT_STORAGE_SIZE/ALIGN; ++i)
        VERIFY(arena.first->storage[ALIGN*i] == (char)i);
    VERIFY(arena.first->next->storage[0] == 'a');

    arena_free(&arena);
}

static void test_normal_allocs() {
    Arena arena;
    char *s1, *s2;

    arena_init(&arena);

    // Allocate the largest block that will fit in a single normal-sized chunk
    // and initialize it.
    s1 = arena_alloc(&arena, DEFAULT_STORAGE_SIZE);
    VERIFY(n_chunks(&arena) == 1);
    memset(s1, 'a', DEFAULT_STORAGE_SIZE);

    // Allocating one more element should allocate another chunk.
    s2 = arena_alloc(&arena, 1);
    *s2 = 'x';
    VERIFY(n_chunks(&arena) == 2);

    // Verify pointers and contents.
    VERIFY(s1 == arena.first->storage);
    VERIFY(s2 == arena.first->next->storage);
    for (size_t i = 0; i < DEFAULT_STORAGE_SIZE; ++i)
        VERIFY(s1[i] == 'a');
    VERIFY(*s2 == 'x');

    arena_free(&arena);
}

static void test_oversized_allocs() {
    Arena arena;
    char *s1, *s2;

    arena_init(&arena);

    // Allocate a block just large enough to need an oversized chunk and
    // initialize it.
    s1 = arena_alloc(&arena, DEFAULT_STORAGE_SIZE + 1);
    VERIFY(chunk_size(arena.cur) == DEFAULT_STORAGE_SIZE + 1);
    VERIFY(n_chunks(&arena) == 2);
    memset(s1, 'a', DEFAULT_STORAGE_SIZE + 1);

    // Allocate a very oversized block and initialize it.
    s2 = arena_alloc(&arena, 10*DEFAULT_STORAGE_SIZE);
    VERIFY(chunk_size(arena.cur) == 10*DEFAULT_STORAGE_SIZE);
    VERIFY(n_chunks(&arena) == 3);
    memset(s2, 'x', 10*DEFAULT_STORAGE_SIZE);

    // Verify pointers and contents. Doing this last could detect memory
    // corruption we'd miss otherwise.
    VERIFY(s1 == arena.first->next->storage);
    VERIFY(s2 == arena.first->next->next->storage);
    for (size_t i = 0; i < DEFAULT_STORAGE_SIZE + 1; ++i)
        VERIFY(s1[i] == 'a');
    for (size_t i = 0; i < 10*DEFAULT_STORAGE_SIZE; ++i)
        VERIFY(s2[i] == 'x');

    arena_free(&arena);
}

static void test_align() {
    Arena arena;

    arena_init(&arena);
    // Verify that allocations return properly aligned pointers.
    for (int i = 1; i <= 3*DEFAULT_STORAGE_SIZE; ++i)
        VERIFY(((uintptr_t)arena_alloc(&arena, i) & (ALIGN - 1)) == 0);
    arena_free(&arena);
}

static void test_cursor_reuse() {
    Arena arena;
    Arena_cursor cursor;
    Chunk *c2, *c3, *c4, *c5;
    char *old_start;
    char *s;

    arena_init(&arena);

    // Allocate three blocks, all slightly smaller than full just to mix things
    // up.
    for (int i = 0; i < 3; ++i)
        arena_alloc(&arena, DEFAULT_STORAGE_SIZE - 1);
    VERIFY(n_chunks(&arena) == 3);

    old_start = arena.cur->start;
    // Get a cursor for the current position.
    arena_get_cursor(&arena, &cursor);

    // Allocate three more blocks.
    for (int i = 0; i < 3; ++i)
        arena_alloc(&arena, DEFAULT_STORAGE_SIZE - 1);
    VERIFY(n_chunks(&arena) == 6);

    // Rewind to the cursor without freeing the memory.
    arena_set_cursor(&arena, &cursor, true);
    // Verify that the 'start' pointer was properly restored.
    VERIFY(arena.cur->start == old_start);

    // Structure should be like this:
    //
    //         |--Allocated---|  |-----Free-----|
    // first -> c1 -> c2 -> c3 -> c4 -> c5 -> c6
    //                      ^
    //                      |
    //                     cur
    //
    // c<n> has <n> bytes free.
    c2 = arena.first->next;
    c3 = c2->next;
    c4 = c3->next;
    c5 = c4->next;

    VERIFY(n_chunks(&arena) == 6);
    VERIFY(arena.cur == c3);
    VERIFY(c5->next->next == NULL);
    VERIFY(c3 == cursor.chunk);
    VERIFY(c3->start == cursor.start);

    // A ten-byte allocation should come from the beginning of c4, giving this:
    //
    //         |-----Allocated------|  |--Free--|
    // first -> c1 -> c2 -> c3 -> c4 -> c5 -> c6
    //                            ^
    //                            |
    //                           cur
    s = arena_alloc(&arena, 10);
    VERIFY(s == c4->storage);
    VERIFY(n_chunks(&arena) == 6);
    VERIFY(arena.cur == c4);
    VERIFY(c2->next == c3 && c3->next == c4 && c4->next == c5);
    VERIFY(c5->next->next == NULL);

    // Another ten-byte allocation should still come from c4.
    s = arena_alloc(&arena, 10);
    VERIFY(s == c4->storage + ALIGN);
    VERIFY(n_chunks(&arena) == 6);
    VERIFY(arena.cur == c4);
    VERIFY(c2->next == c3 && c3->next == c4 && c4->next == c5);
    VERIFY(c5->next->next == NULL);

    // An oversized allocation now should give this:
    //
    //         |----------Allocated----------|  |--Free--|
    // first -> c1 -> c2 -> c3 -> c4 -> <new> -> c5 -> c6
    //                                    ^
    //                                    |
    //                                   cur
    s = arena_alloc(&arena, DEFAULT_STORAGE_SIZE + 1);
    VERIFY(s == arena.cur->storage);
    VERIFY(n_chunks(&arena) == 7);
    VERIFY(arena.cur != c4 && arena.cur != c5);
    VERIFY(c4->next->next = c5);

    arena_free(&arena);
}

static void test_cursor_no_reuse() {
    Arena arena;
    Arena_cursor cursor;
    Chunk *c2, *c3;
    char *old_start;

    arena_init(&arena);

    // Allocate three blocks, all slightly smaller than full just to mix things
    // up.
    for (int i = 0; i < 3; ++i)
        arena_alloc(&arena, DEFAULT_STORAGE_SIZE - 1);

    old_start = arena.cur->start;
    // Get a cursor for the current position.
    arena_get_cursor(&arena, &cursor);

    // Allocate three more blocks.
    for (int i = 0; i < 3; ++i)
        arena_alloc(&arena, DEFAULT_STORAGE_SIZE - 1);

    // Structure is like this:
    //
    // first -> c1 -> c2 -> c3 -> c4 -> c5 -> c6
    //                                        ^
    //                                        |
    //                                       cur
    c2 = arena.first->next;
    c3 = c2->next;

    // Rewind the cursor and free memory.
    arena_set_cursor(&arena, &cursor, false);
    // Verify that the 'start' pointer was properly restored.
    VERIFY(arena.cur->start == old_start);

    // Structure should be like this:
    //
    // first -> c1 -> c2 -> c3
    //                      ^
    //                      |
    //                     cur
    VERIFY(n_chunks(&arena) == 3);
    VERIFY(arena.cur == c3);
    VERIFY(c2->next == c3);
    VERIFY(c3->next == NULL);

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
    test_empty();
    test_one_byte_allocs();
    test_normal_allocs();
    test_oversized_allocs();
    test_align();
    test_cursor_reuse();
    test_cursor_no_reuse();
    test_strdup();
    test_strndup();
}
