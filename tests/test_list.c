#include "common.h"
#include "list.h"

#define MAKE_LIST(name, ...)                           \
  List name;                                           \
                                                       \
  list_make(&name, N_ARGS(__VA_ARGS__), ##__VA_ARGS__)

#define VERIFY_LIST_EQUALS(list, ...)                            \
  VERIFY(list_equals(&list, N_ARGS(__VA_ARGS__), ##__VA_ARGS__))

#define VERIFY_NOT_LIST_EQUALS(list, ...)                         \
  VERIFY(!list_equals(&list, N_ARGS(__VA_ARGS__), ##__VA_ARGS__))

static void test_add() {
    // Also tests list_equals().
    List list;

    list_init(&list);

    // Verify that the list is empty.
    VERIFY_LIST_EQUALS(list);
    list_add(&list, 10);
    // Verify that the list is not empty.
    VERIFY_NOT_LIST_EQUALS(list);
    VERIFY_LIST_EQUALS(list, 10);
    list_add(&list, 20);
    VERIFY_LIST_EQUALS(list, 20, 10);
    VERIFY_NOT_LIST_EQUALS(list, 20);
    VERIFY_NOT_LIST_EQUALS(list, 20, 10, 0);

    list_free(&list);
}

static void test_make() {
    #define VERIFY_MAKE(...)                       \
      {                                            \
          MAKE_LIST(list, ##__VA_ARGS__);          \
                                                   \
          VERIFY_LIST_EQUALS(list, ##__VA_ARGS__); \
          list_free(&list);                        \
      }

    VERIFY_MAKE(); // Empty list.
    VERIFY_MAKE(10);
    VERIFY_MAKE(10, 20);
    VERIFY_MAKE(10, 20, 30);

    #undef VERIFY_MAKE
}

static void test_is_sorted() {
    #define VERIFY_SORTED(sorted, ...)             \
      {                                            \
          MAKE_LIST(list, ##__VA_ARGS__);          \
                                                   \
          VERIFY(sorted == list_is_sorted(&list)); \
          list_free(&list);                        \
      }

    VERIFY_SORTED(true); // Empty list should be sorted.
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

    MAKE_LIST(list, 10, 20, 20, 10);

    VERIFY_REMOVE(15,  10, 20, 20, 10);
    VERIFY_REMOVE(10,  20, 20, 10);
    VERIFY_REMOVE(10,  20, 20);
    VERIFY_REMOVE(20,  20);
    VERIFY_REMOVE(20); // Empty?
    VERIFY_REMOVE(10); // Empty?

    list_free(&list);

    #undef VERIFY_REMOVE
}

static void test_remove_all() {
    #define VERIFY_REMOVE_ALL(to_remove, ...)  \
      list_remove_all(&list, to_remove);       \
      VERIFY_LIST_EQUALS(list, ##__VA_ARGS__);

    MAKE_LIST(list, 1, 2, 1, 1, 2, 2, 10, 1, 1, 1, 2, 2, 2);

    VERIFY_REMOVE_ALL(5,  1, 2, 1, 1, 2, 2, 10, 1, 1, 1, 2, 2, 2);
    VERIFY_REMOVE_ALL(1,  2, 2, 2, 10, 2, 2, 2);
    VERIFY_REMOVE_ALL(2,  10);
    VERIFY_REMOVE_ALL(10); // Empty?
    VERIFY_REMOVE_ALL(1); // Empty?

    list_free(&list);

    #undef VERIFY_REMOVE_ALL
}

static void test_reverse() {
    #define VERIFY_REVERSE_SET_LIST(...)           \
      {                                            \
          MAKE_LIST(list, ##__VA_ARGS__);          \
                                                   \
          list_reverse(&list)
    #define VERIFY_REVERSE_RESULT(...)             \
          VERIFY_LIST_EQUALS(list, ##__VA_ARGS__); \
          list_free(&list);                        \
      }

    VERIFY_REVERSE_SET_LIST(); // Empty.
    VERIFY_REVERSE_RESULT(); // Empty?

    VERIFY_REVERSE_SET_LIST(10);
    VERIFY_REVERSE_RESULT(10);

    VERIFY_REVERSE_SET_LIST(10, 20);
    VERIFY_REVERSE_RESULT(20, 10);

    VERIFY_REVERSE_SET_LIST(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    VERIFY_REVERSE_RESULT(9, 8, 7, 6, 5, 4, 3, 2, 1, 0);

    #undef VERIFY_REVERSE_SET_LIST
    #undef VERIFY_REVERSE_RESULT
}

static void test_sort(void sort_fn(List *)) {
    // Assume selection sort is correct and test other sorting algorithms by
    // comparing against it.
    #define VERIFY_SORT(...)             \
      {                                  \
          MAKE_LIST(l1, ##__VA_ARGS__);  \
          MAKE_LIST(l2, ##__VA_ARGS__);  \
                                         \
          list_selection_sort(&l1);      \
          sort_fn(&l2);                  \
          VERIFY(lists_equal(&l1, &l2)); \
          list_free(&l1);                \
          list_free(&l2);                \
      }

    VERIFY_SORT(); // Empty list should sort correctly too.
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

void test_list() {
    test_add();
    test_make();
    test_is_sorted();
    test_remove();
    test_remove_all();
    test_reverse();
    test_sort(list_selection_sort);
    test_sort(list_insertion_sort);
    test_sort(list_mergesort);
}
