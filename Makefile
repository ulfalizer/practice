c_files := $(addprefix src/, algo.c common.c eval.c hash_table.c list.c \
	min_heap.c min_max_stack.c queue.c rot_tree.c search_tree.c sort.c \
	stack.c test.c test_algo.c test_eval.c test_hash_table.c test_list.c \
	test_min_heap.c test_min_max_stack.c test_queue.c test_rot_tree.c \
	test_search_tree.c test_tree.c test_sort.c test_vector.c tree.c vector.c)
h_files := $(addprefix src/, algo.h common.h hash_table.h list.h min_heap.h \
	min_max_stack.h queue.h rot_tree.h search_tree.h sort.h stack.h tree.h \
	vector.h)

test: $(c_files) $(h_files)
	gcc -std=gnu11 -Og -g -Wall -o $@ $(c_files) -lm

.PHONY: clean
clean:
	rm test
