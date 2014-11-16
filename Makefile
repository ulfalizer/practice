c_files := algo.c common.c hash_table.c list.c min_heap.c queue.c sort.c \
	test.c test_algo.c test_hash_table.c test_list.c test_min_heap.c \
	test_queue.c test_tree.c test_sort.c test_vector.c tree.c vector.c
h_files := algo.h common.h hash_table.h list.h min_heap.h queue.h sort.h \
	tree.h vector.h

test: $(c_files) $(h_files)
	gcc -std=gnu11 -g -Wall -o $@ $(c_files)

.PHONY: clean
clean:
	rm test
