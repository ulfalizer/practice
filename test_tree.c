#include "common.h"
#include "tree.h"
#include "vector.h"

// Helper for specifying the lack of a node with tree_make()
static int _ = 0xDEAD;

#define MAKE_TREE(...) \
  tree_make(N_ARGS(__VA_ARGS__), ##__VA_ARGS__)

// Tests both tree_equals() and trees_equal()
static void test_equals() {
    // Specifies the tree to use for the tests
    #define TEST_EQUALS_TREE(...)                           \
      do {                                                  \
          Tree_node *tree = MAKE_TREE(__VA_ARGS__), *tree2
    // Verifies that the tree equals (equals == true) or does not equal the
    // given tree
    #define TEST_EQUALS(equals, ...)                                \
          /* Test tree_equals() */                                  \
          VERIFY(equals ==                                          \
            tree_equals(tree, N_ARGS(__VA_ARGS__), ##__VA_ARGS__)); \
          /* Test trees_equal() */                                  \
          tree2 = MAKE_TREE(__VA_ARGS__);                           \
          VERIFY(equals == trees_equal(tree, tree2));               \
          tree_free(tree2)
    // Ends the test
    #define TEST_EQUALS_END \
          tree_free(tree);  \
      }                     \
      while (0)

    // Empty tree
    TEST_EQUALS_TREE();
    TEST_EQUALS(true);
    TEST_EQUALS(true, _);
    TEST_EQUALS(true, _, _);
    TEST_EQUALS(false, 1);
    TEST_EQUALS(false, 1, _);
    TEST_EQUALS(false, 1, _,_);
    TEST_EQUALS_END;

    // One-element tree
    TEST_EQUALS_TREE(1);
    TEST_EQUALS(true, 1);
    TEST_EQUALS(true, 1, _);
    TEST_EQUALS(true, 1, _,_);
    TEST_EQUALS(false);
    TEST_EQUALS(false, 2);
    TEST_EQUALS(false, 2, _);
    TEST_EQUALS(false,
       1,
      1);
    TEST_EQUALS(false,
       1,
      _,1);
    TEST_EQUALS_END;

    // Two-element tree
    TEST_EQUALS_TREE(
       1,
      2);
    TEST_EQUALS(true,
       1,
      2);
    TEST_EQUALS(true,
       1,
      2,_);
    TEST_EQUALS(true,
         1,
       2,  _,
      _);
    TEST_EQUALS(false);
    TEST_EQUALS(false, 1);
    TEST_EQUALS(false,
       1,
      1);
    TEST_EQUALS(false,
       2,
      2);
    TEST_EQUALS(false,
       1,
      _,2);
    TEST_EQUALS_END;

    // Three-element tree
    TEST_EQUALS_TREE(
       1,
      2,3);
    TEST_EQUALS(true,
       1,
      2,3);
    TEST_EQUALS(true,
         1,
       2,  3,
      _);
    TEST_EQUALS(false,
       1,
      _,3);
    TEST_EQUALS(false,
       1,
      2,_);
    TEST_EQUALS(false,
       1,
      2,1);
    TEST_EQUALS_END;

    // Complex tree
    TEST_EQUALS_TREE(
             0,
         1,      2,
       3,  4,  _,  5,
      _,6,_,7,_,_,8,9);
    TEST_EQUALS(true,
             0,
         1,      2,
       3,  4,  _,  5,
      _,6,_,7,_,_,8,9);
    TEST_EQUALS(false,
             0,
         1,      2,
       3,  4,  _,  5,
      _,6,1,7,_,_,8,9);
    TEST_EQUALS(false,
             0,
         1,      2,
       3,  4,  _,  5,
      _,6,_,7,_,_,8,_);
    TEST_EQUALS(false,
                     0,
             1,              2,
         3,      4,      _,      5,
       _,  6,  _,  7,  _,  _,  8,  9,
      _,_,0);
    TEST_EQUALS_END;

    #undef TEST_EQUALS_TREE
    #undef TEST_EQUALS
    #undef TEST_EQUALS_END
}

static void verify_vector_equals_helper(Vector *v, size_t len, ...) {
    va_list ap;
    VERIFY(vector_len(v) == len);
    va_start(ap, len);
    for (size_t i = 0; i < len; ++i)
        VERIFY(((Tree_node*)vector_get(v, i))->key == va_arg(ap, int));
    va_end(ap);
}

#define VERIFY_VECTOR_EQUALS(v, ...) \
  verify_vector_equals_helper(&v, N_ARGS(__VA_ARGS__), ##__VA_ARGS__)

#define TRAVERSE_TEST_TREE(fn, ...)             \
  do {                                          \
      Vector v;                                 \
      Tree_node *tree;                          \
                                                \
      vector_init(&v);                          \
      tree = MAKE_TREE(__VA_ARGS__);            \
      fn(tree, &v)
#define TRAVERSE_TEST_RES(...)                \
      VERIFY_VECTOR_EQUALS(v, ##__VA_ARGS__); \
      vector_free(&v);                        \
      tree_free(tree);                        \
  }                                           \
  while (0)

static void test_dfs() {
    TRAVERSE_TEST_TREE(tree_nodes_to_vector_dfs);
    TRAVERSE_TEST_RES();

    TRAVERSE_TEST_TREE(tree_nodes_to_vector_dfs,
      1);
    TRAVERSE_TEST_RES(1);

    TRAVERSE_TEST_TREE(tree_nodes_to_vector_dfs,
       1,
      2);
    TRAVERSE_TEST_RES(2, 1);

    TRAVERSE_TEST_TREE(tree_nodes_to_vector_dfs,
       1,
      2,3);
    TRAVERSE_TEST_RES(2, 1, 3);

    TRAVERSE_TEST_TREE(tree_nodes_to_vector_dfs,
         1,
       2,  3,
      4,5,6,7);
    TRAVERSE_TEST_RES(4, 2, 5, 1, 6, 3, 7);

    TRAVERSE_TEST_TREE(tree_nodes_to_vector_dfs,
         1,
       2,  3,
      _,5,6,_);
    TRAVERSE_TEST_RES(2, 5, 1, 6, 3);

    TRAVERSE_TEST_TREE(tree_nodes_to_vector_dfs,
             0,
         1,      2,
       3,  4,  _,  5,
      _,6,_,7,_,_,8,9);
    TRAVERSE_TEST_RES(3, 6, 1, 4, 7, 0, 2, 8, 5, 9);
}

static void test_bfs() {
    TRAVERSE_TEST_TREE(tree_nodes_to_vector_bfs);
    TRAVERSE_TEST_RES();

    TRAVERSE_TEST_TREE(tree_nodes_to_vector_bfs,
      1);
    TRAVERSE_TEST_RES(1);

    TRAVERSE_TEST_TREE(tree_nodes_to_vector_bfs,
       1,
      2);
    TRAVERSE_TEST_RES(1, 2);

    TRAVERSE_TEST_TREE(tree_nodes_to_vector_bfs,
       1,
      2,3);
    TRAVERSE_TEST_RES(1, 2, 3);

    TRAVERSE_TEST_TREE(tree_nodes_to_vector_bfs,
         1,
       2,  3,
      4,5,6,7);
    TRAVERSE_TEST_RES(1, 2, 3, 4, 5, 6, 7);

    TRAVERSE_TEST_TREE(tree_nodes_to_vector_bfs,
         1,
       2,  3,
      _,5,6,_);
    TRAVERSE_TEST_RES(1, 2, 3, 5, 6);

    TRAVERSE_TEST_TREE(tree_nodes_to_vector_bfs,
             0,
         1,      2,
       3,  4,  _,  5,
      _,6,_,7,_,_,8,9);
    TRAVERSE_TEST_RES(0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
}

#undef TRAVERSE_TEST_TREE
#undef TRAVERSE_TEST_RES

static void test_valid_bin_search_tree() {
    #define VERIFY_BIN_SEARCH_TREE(valid, ...)          \
      {                                                 \
          Tree_node *tree = MAKE_TREE(__VA_ARGS__);     \
          VERIFY(valid == valid_bin_search_tree(tree)); \
          tree_free(tree);                              \
      }

    VERIFY_BIN_SEARCH_TREE(true);
    VERIFY_BIN_SEARCH_TREE(true,
      1);
    VERIFY_BIN_SEARCH_TREE(true,
       2,
      1);
    VERIFY_BIN_SEARCH_TREE(true,
       1,
      _,2);
    VERIFY_BIN_SEARCH_TREE(true,
       2,
      1,3);
    VERIFY_BIN_SEARCH_TREE(true,
         3,
       1,  5,
      _,_,4,_);
    VERIFY_BIN_SEARCH_TREE(true,
         3,
       1,  5,
      _,2);
    VERIFY_BIN_SEARCH_TREE(true,
             1,
      INT_MIN);
    VERIFY_BIN_SEARCH_TREE(true,
       1,
      _,INT_MAX);
    VERIFY_BIN_SEARCH_TREE(true,
             1,
      INT_MIN,INT_MAX);
    VERIFY_BIN_SEARCH_TREE(true,
      INT_MIN,
        _,1);
    VERIFY_BIN_SEARCH_TREE(true,
      INT_MAX,
        1);

    VERIFY_BIN_SEARCH_TREE(false,
       1,
      2);
    VERIFY_BIN_SEARCH_TREE(false,
       2,
      _,1);
    VERIFY_BIN_SEARCH_TREE(false,
       2,
      2,3);
    VERIFY_BIN_SEARCH_TREE(false,
       2,
      1,2);
    VERIFY_BIN_SEARCH_TREE(false,
         3,
       1,  5,
      _,_,2,_);
    VERIFY_BIN_SEARCH_TREE(false,
         3,
       1,  5,
      _,4);
    VERIFY_BIN_SEARCH_TREE(false,
       INT_MIN,
      INT_MIN);
    VERIFY_BIN_SEARCH_TREE(false,
      INT_MAX,
        _,INT_MAX);

    #undef VERIFY_BIN_SEARCH_TREE
}

void test_tree() {
    test_equals();
    test_dfs();
    test_bfs();
    test_valid_bin_search_tree();
}
