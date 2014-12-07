#include "common.h"

void test_algo();
void test_common();
void test_embedded_list();
void test_eval();
void test_hash_table();
void test_list();
void test_min_heap();
void test_min_max_stack();
void test_queue();
void test_rot_tree();
void test_search_tree();
void test_string();
void test_sort();
void test_tree();
void test_vector();

int main() {
    test_algo();
    test_common();
    test_embedded_list();
    test_eval();
    test_hash_table();
    test_list();
    test_min_heap();
    test_min_max_stack();
    test_queue();
    test_rot_tree();
    test_search_tree();
    test_sort();
    test_string();
    test_tree();
    test_vector();
    puts("all tests passed");
}
