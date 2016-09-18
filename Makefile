ifeq ($(origin CC), default)
  CC = gcc
endif
CFLAGS ?= -g -Og
# We strictly only need -fno-strict-aliasing for test_embedded_list.c
override CFLAGS += -std=gnu11 -fno-strict-aliasing -Wall -Wextra \
  -Wno-sign-compare -Wredundant-decls -Wstrict-prototypes
override CPPFLAGS += -Iinclude
override LDLIBS += -lm

src_files = src/algo.c \
            src/arena_allocator.c \
            src/common.c \
            src/compact_vector.c \
            src/dynamic_string.c \
            src/eval.c \
            src/hash_table.c \
            src/list.c \
            src/min_heap.c \
            src/min_max_stack.c \
            src/queue.c \
            src/regex_compiler.c \
            src/rot_tree.c \
            src/search_tree.c \
            src/sort.c \
            src/stack.c \
            src/tree.c \
            src/utf8.c \
            src/vector.c \
            tests/test.c \
            tests/test_algo.c \
            tests/test_arena_allocator.c \
            tests/test_common.c \
            tests/test_compact_vector.c \
            tests/test_dynamic_string.c \
            tests/test_embedded_list.c \
            tests/test_eval.c \
            tests/test_hash_table.c \
            tests/test_list.c \
            tests/test_min_heap.c \
            tests/test_min_max_stack.c \
            tests/test_queue.c \
            tests/test_regex_compiler.c \
            tests/test_rot_tree.c \
            tests/test_search_tree.c \
            tests/test_sort.c \
            tests/test_tree.c \
            tests/test_utf8.c \
            tests/test_vector.c

headers = include/algo.h \
          include/arena_allocator.h \
          include/common.h \
          include/compact_vector.h \
          include/dynamic_string.h \
          include/embedded_list.h \
          include/eval.h \
          include/hash_table.h \
          include/list.h \
          include/min_heap.h \
          include/min_max_stack.h \
          include/queue.h \
          include/regex_compiler.h \
          include/rot_tree.h \
          include/search_tree.h \
          include/sort.h \
          include/stack.h \
          include/tree.h \
          include/utf8.h \
          include/vector.h

# This is a quick compile. Keep it simple.
test: $(src_files) $(headers)
	$(CC) -o $@ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(src_files) $(LDLIBS)

.PHONY: clean
clean:
	$(RM) test
