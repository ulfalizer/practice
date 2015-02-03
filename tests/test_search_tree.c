#include "common.h"
#include "search_tree.h"

// Assignments of 234 in the tests below are for verifying that arguments are
// only modified when they should be.

static void populate(Search_tree *tree) {
    #define ADD_DOUBLE(n)                  \
      search_tree_set(tree, n, 2*n, NULL); \
      VERIFY(search_tree_valid(tree));

    ADD_DOUBLE(5);
    ADD_DOUBLE(9);
    ADD_DOUBLE(2);
    ADD_DOUBLE(7);
    ADD_DOUBLE(4);
    ADD_DOUBLE(3);
    ADD_DOUBLE(6);
    ADD_DOUBLE(1);
    ADD_DOUBLE(8);
    ADD_DOUBLE(0);

    #undef ADD_DOUBLE

    search_tree_set(tree, INT_MAX, 100, NULL);
    VERIFY(search_tree_valid(tree));
    search_tree_set(tree, INT_MIN, 200, NULL);
    VERIFY(search_tree_valid(tree));
}

static void test_set_get_helper(bool keys_exist) {
    #define VERIFY_SET_GET(key, val)                         \
      {                                                      \
          int v = 234;                                       \
                                                             \
          if (!keys_exist) {                                 \
              VERIFY(!search_tree_get(&tree, key, &v));      \
              VERIFY(v == 234);                              \
              VERIFY(!search_tree_set(&tree, key, val, &v)); \
              VERIFY(v == 234);                              \
          }                                                  \
          VERIFY(search_tree_get(&tree, key, &v));           \
          VERIFY(v == val);                                  \
          v = 234;                                           \
          VERIFY(search_tree_set(&tree, key, val + 1, &v));  \
          VERIFY(v == val);                                  \
          VERIFY(search_tree_get(&tree, key, &v));           \
          VERIFY(v == val + 1);                              \
          VERIFY(search_tree_get(&tree, key, NULL));         \
          VERIFY(search_tree_valid(&tree));                  \
      }

    #define VERIFY_SET_GET_DOUBLE(key) \
      VERIFY_SET_GET(key, 2*key)

    Search_tree tree;
    search_tree_init(&tree);

    if (keys_exist)
        populate(&tree);
    for (int i = 0; i < 10; ++i)
        VERIFY_SET_GET_DOUBLE(i);
    VERIFY_SET_GET(INT_MAX, 100);
    VERIFY_SET_GET(INT_MIN, 200);
    search_tree_free(&tree);

    #undef VERIFY_SET_GET
    #undef VERIFY_SET_GET_DOUBLE
}

static void test_remove(void) {
    Search_tree tree;

    search_tree_init(&tree);
    populate(&tree);

    #define VERIFY_REMOVE_NOT_EXISTS(key)              \
      {                                                \
          int v = 234;                                 \
                                                       \
          VERIFY(!search_tree_get(&tree, key, &v));    \
          VERIFY(!search_tree_remove(&tree, key, &v)); \
          VERIFY(!search_tree_get(&tree, key, &v));    \
          VERIFY(v == 234);                            \
      }

    #define VERIFY_REMOVE_EXISTS(key)                    \
      {                                                  \
          int v1, v2;                                    \
                                                         \
          VERIFY(search_tree_get(&tree, key, &v1));      \
          VERIFY(search_tree_remove(&tree, key, &v2));   \
          VERIFY(!search_tree_get(&tree, key, NULL));    \
          VERIFY(!search_tree_remove(&tree, key, NULL)); \
          VERIFY(v1 == v2);                              \
      }

    VERIFY_REMOVE_NOT_EXISTS(10);
    VERIFY_REMOVE_NOT_EXISTS(-10);
    for (int i = 0; i < 10; ++i)
        VERIFY_REMOVE_EXISTS(i);
    VERIFY_REMOVE_EXISTS(INT_MIN);
    VERIFY_REMOVE_EXISTS(INT_MAX);

    #undef VERIFY_REMOVE_NOT_EXISTS
    #undef VERIFY_REMOVE_EXISTS
}

static void test_set_get(void) {
    test_set_get_helper(false);
    test_set_get_helper(true);
}

void test_search_tree(void) {
    test_set_get();
    test_remove();
}
