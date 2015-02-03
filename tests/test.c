#include "common.h"

void test_algo(void);
void test_arena_allocator(void);
void test_common(void);
void test_compact_vector(void);
void test_embedded_list(void);
void test_eval(void);
void test_hash_table(void);
void test_list(void);
void test_min_heap(void);
void test_min_max_stack(void);
void test_queue(void);
void test_regex(void);
void test_rot_tree(void);
void test_search_tree(void);
void test_string(void);
void test_sort(void);
void test_tree(void);
void test_utf8(void);
void test_vector(void);

int main(void) {
    test_algo();
    test_arena_allocator();
    test_common();
    test_compact_vector();
    test_embedded_list();
    test_eval();
    test_hash_table();
    test_list();
    test_min_heap();
    test_min_max_stack();
    test_queue();
    test_regex();
    test_rot_tree();
    test_search_tree();
    test_sort();
    test_string();
    test_tree();
    test_utf8();
    test_vector();

    puts("all tests passed");
}
