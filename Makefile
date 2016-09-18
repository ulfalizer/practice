CC ?= gcc
EXECUTABLE = test

sources = $(addprefix src/, algo.c arena_allocator.c common.c \
  compact_vector.c eval.c hash_table.c list.c min_heap.c min_max_stack.c \
  queue.c regex_compiler.c rot_tree.c search_tree.c sort.c stack.c \
  dynamic_string.c tree.c utf8.c vector.c)

tests = $(addprefix tests/, test.c test_algo.c test_arena_allocator.c \
  test_common.c test_compact_vector.c test_embedded_list.c test_eval.c \
  test_hash_table.c test_list.c test_min_heap.c test_min_max_stack.c \
  test_queue.c test_regex_compiler.c test_rot_tree.c test_search_tree.c \
  test_sort.c test_dynamic_string.c test_tree.c test_utf8.c test_vector.c)

headers = $(addprefix include/, algo.h arena_allocator.h common.h \
  compact_vector.h embedded_list.h hash_table.h list.h min_heap.h \
  min_max_stack.h queue.h regex_compiler.h rot_tree.h search_tree.h sort.h \
  stack.h dynamic_string.h tree.h utf8.h vector.h)

warnings = -Wall -Wextra -Wno-sign-compare -Wredundant-decls \
  -Wstrict-prototypes

$(EXECUTABLE): $(sources) $(tests) $(headers)
# We strictly only need -fno-strict-aliasing for test_embedded_list.c.
	$(CC) -std=gnu11 -g -Og -fno-strict-aliasing $(warnings) -Iinclude $(sources) $(tests) -lm -o $@

.PHONY: clean
clean:
	rm $(EXECUTABLE)
