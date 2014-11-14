#include "common.h"
#include "list.h"

#define MAKE_LIST(...) \
  list_make(N_ARGS(__VA_ARGS__), ##__VA_ARGS__)

#define VERIFY_LIST_EQUALS(list, ...) \
  VERIFY(list_equals(list, N_ARGS(__VA_ARGS__), ##__VA_ARGS__))

#define VERIFY_NOT_LIST_EQUALS(list, ...) \
  VERIFY(!list_equals(list, N_ARGS(__VA_ARGS__), ##__VA_ARGS__))

static void test_add() {
    // Also tests list_equals()
    Node *list = MAKE_LIST();
    VERIFY_LIST_EQUALS(list);

    list_add(&list, 10);
    VERIFY_NOT_LIST_EQUALS(list);
    VERIFY_LIST_EQUALS(list, 10);
    list_add(&list, 20);
    VERIFY_LIST_EQUALS(list, 20, 10);
    VERIFY_NOT_LIST_EQUALS(list, 20);
    VERIFY_NOT_LIST_EQUALS(list, 20, 10, 0);
    list_free(list);
}

static void test_make() {
    #define VERIFY_MAKE(...)                   \
      {                                        \
      Node *list = MAKE_LIST(__VA_ARGS__);     \
      VERIFY_LIST_EQUALS(list, ##__VA_ARGS__); \
      list_free(list);                         \
      }

    VERIFY_MAKE();
    VERIFY_MAKE(10);
    VERIFY_MAKE(10, 20);
    VERIFY_MAKE(10, 20, 30);

    #undef VERIFY_MAKE
}

static void test_is_sorted() {
    #define VERIFY_SORTED(sorted, ...)        \
      {                                       \
      Node *list = MAKE_LIST(__VA_ARGS__);    \
      VERIFY(sorted == list_is_sorted(list)); \
      list_free(list);                        \
      }

    VERIFY_SORTED(true);
    VERIFY_SORTED(true, 1);
    VERIFY_SORTED(true, 1, 2);
    VERIFY_SORTED(true, 1, 1);
    VERIFY_SORTED(true, 1, 1, 2);
    VERIFY_SORTED(false, 2, 1);
    VERIFY_SORTED(false, 2, 2, 1);

    #undef VERIFY_SORTED
}

static void test_remove() {
    #define VERIFY_REMOVE(to_remove, ...)      \
      list_remove(&list, to_remove);           \
      VERIFY_LIST_EQUALS(list, ##__VA_ARGS__);

    Node *list = MAKE_LIST(10, 20, 20, 10);
    VERIFY_REMOVE(15,  10, 20, 20, 10);
    VERIFY_REMOVE(10,  20, 20, 10);
    VERIFY_REMOVE(10,  20, 20);
    VERIFY_REMOVE(20,  20);
    VERIFY_REMOVE(20);
    VERIFY_REMOVE(10);

    #undef VERIFY_REMOVE
}

static void test_remove_all() {
    #define VERIFY_REMOVE_ALL(to_remove, ...)  \
      list_remove_all(&list, to_remove);       \
      VERIFY_LIST_EQUALS(list, ##__VA_ARGS__);

    Node *list = MAKE_LIST(1, 2, 1, 1, 2, 2, 10, 1, 1, 1, 2, 2, 2);
    VERIFY_REMOVE_ALL(5,  1, 2, 1, 1, 2, 2, 10, 1, 1, 1, 2, 2, 2);
    VERIFY_REMOVE_ALL(1,  2, 2, 2, 10, 2, 2, 2);
    VERIFY_REMOVE_ALL(2,  10);
    VERIFY_REMOVE_ALL(10);
    VERIFY_REMOVE_ALL(1);

    #undef VERIFY_REMOVE_ALL
}

static void test_reverse() {
    #define VERIFY_REVERSE(...)                \
      list_reverse(&list);                     \
      VERIFY_LIST_EQUALS(list, ##__VA_ARGS__); \
      list_free(list);

    Node *list = MAKE_LIST();
    VERIFY_REVERSE();
    list = MAKE_LIST(10);
    VERIFY_REVERSE(10);
    list = MAKE_LIST(10, 20);
    VERIFY_REVERSE(20, 10);
    list = MAKE_LIST(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    VERIFY_REVERSE(9, 8, 7, 6, 5, 4, 3, 2, 1, 0);

    #undef VERIFY_REVERSE
}

static void test_sort(void sort_fn(Node **)) {
    #define VERIFY_SORT(...) \
      { \
          Node *l1 = MAKE_LIST(__VA_ARGS__); \
          Node *l2 = MAKE_LIST(__VA_ARGS__); \
          list_selection_sort(&l1);          \
          sort_fn(&l2);                      \
          VERIFY(lists_equal(l1, l2));       \
          list_free(l1);                     \
          list_free(l2);                     \
      }

    VERIFY_SORT();
    VERIFY_SORT(1);
    VERIFY_SORT(1, 1);
    VERIFY_SORT(1, 2);
    VERIFY_SORT(2, 1);
    VERIFY_SORT(1, 1, 1);
    VERIFY_SORT(1, 2, 3);
    VERIFY_SORT(1, 3, 2);
    VERIFY_SORT(2, 1, 3);
    VERIFY_SORT(2, 3, 1);
    VERIFY_SORT(3, 1, 2);
    VERIFY_SORT(3, 2, 1);
    VERIFY_SORT(1, 2, 3, 4);
    VERIFY_SORT(4, 3, 2, 1);
    VERIFY_SORT(8, 1, 7, 2, -5, 6, 3, 5, 4, 1, 1, 2);

    #undef VERIFY_SORT
}

static void test_merge() {
    #define VERIFY_MERGE(...)                   \
      list_merge(&list1, list2);                \
      VERIFY_LIST_EQUALS(list1, ##__VA_ARGS__); \
      list_free(list1);

    Node *list1 = MAKE_LIST(), *list2 = MAKE_LIST();
    VERIFY_MERGE();
    list1 = MAKE_LIST(10);
    list2 = MAKE_LIST();
    VERIFY_MERGE(10);
    list1 = MAKE_LIST();
    list2 = MAKE_LIST(10);
    VERIFY_MERGE(10);
    list1 = MAKE_LIST(10);
    list2 = MAKE_LIST(10);
    VERIFY_MERGE(10, 10);
    list1 = MAKE_LIST(10);
    list2 = MAKE_LIST(20);
    VERIFY_MERGE(10, 20);
    list1 = MAKE_LIST(20);
    list2 = MAKE_LIST(10);
    VERIFY_MERGE(10, 20);
    list1 = MAKE_LIST(10, 30, 50, 60);
    list2 = MAKE_LIST(20, 40, 70);
    VERIFY_MERGE(10, 20, 30, 40, 50, 60, 70);

    #undef VERIFY_MERGE
}

static void test_split() {
    Node *list;
    Node *other;

    list = MAKE_LIST(10);
    other = list_split(list);
    VERIFY_LIST_EQUALS(list, 10);
    VERIFY(other == NULL);
    list_free(list);
    list_free(other);

    list = MAKE_LIST(10, 20);
    other = list_split(list);
    VERIFY_LIST_EQUALS(list, 10);
    VERIFY_LIST_EQUALS(other, 20);
    list_free(list);
    list_free(other);

    list = MAKE_LIST(10, 20, 30);
    other = list_split(list);
    VERIFY_LIST_EQUALS(list, 10, 20);
    VERIFY_LIST_EQUALS(other, 30);
    list_free(list);
    list_free(other);
}

void test_list() {
    test_add();
    test_make();
    test_is_sorted();
    test_remove();
    test_remove_all();
    test_reverse();
    test_sort(list_selection_sort);
    test_sort(list_insertion_sort);
    test_merge();
    test_split();
    test_sort(list_mergesort);
}
