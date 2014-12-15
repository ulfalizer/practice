sources := $(addprefix src/, algo.c arena_allocator.c common.c eval.c \
  hash_table.c list.c min_heap.c min_max_stack.c queue.c rot_tree.c \
  search_tree.c sort.c stack.c string_builder.c tree.c vector.c)

tests := $(addprefix tests/, test.c test_algo.c test_arena_allocator.c \
  test_common.c test_embedded_list.c test_eval.c test_hash_table.c \
  test_list.c test_min_heap.c test_min_max_stack.c test_queue.c \
  test_rot_tree.c test_search_tree.c test_sort.c test_string_builder.c \
  test_tree.c test_vector.c)

headers := $(addprefix src/, algo.h arena_allocator.h common.h \
  embedded_list.h hash_table.h list.h min_heap.h min_max_stack.h queue.h \
  rot_tree.h search_tree.h sort.h stack.h tree.h vector.h)

test: $(sources) $(tests) $(headers)
# We strictly only need -fno-strict-aliasing for test_embedded_list.c.
	gcc -std=gnu11 -g -Og -fno-strict-aliasing -Wall -Isrc/ -o $@ $(sources) $(tests) -lm

.PHONY: clean
clean:
	rm test
