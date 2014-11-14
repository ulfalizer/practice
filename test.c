#include "common.h"

void test_algo();
void test_hash_table();
void test_list();
void test_min_heap();
void test_queue();
void test_sort();

int main() {
    test_algo();
    test_hash_table();
    test_list();
    test_min_heap();
    test_queue();
    test_sort();
    puts("all tests passed");
}
