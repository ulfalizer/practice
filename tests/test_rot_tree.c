#include "common.h"
#include "rot_tree.h"
#include "tree.h"

// Assignments of 234 in the tests below are for verifying that arguments are
// only modified when they should be

// Helper for specifying the lack of a node with tree_make()
static const int _ = 0xDEAD;

static void test_structure(void)
{
    // White-box testing of the internal structure after various operations

    // Set the tree to use for the following tests
    #define SET_TREE(...)                                                     \
      do {                                                                    \
          Rot_tree tree;                                                      \
          Tree_node *tmp;                                                     \
                                                                              \
          tmp = tree_make(N_ARGS(__VA_ARGS__), ##__VA_ARGS__)
    // Apply 'op' to (a copy of) the tree and verify that the structure of the
    // resulting tree matches the remaining arguments
    #define VERIFY_STRUCT(op, ...)                                            \
          tree.root = tree_copy(tmp);                                         \
          op;                                                                 \
          VERIFY(tree_equals(tree.root, N_ARGS(__VA_ARGS__), ##__VA_ARGS__)); \
          rot_tree_free(&tree)
    // End the test (so we can set a new tree to test)
    #define END_TEST                                                          \
          tree_free(tmp);                                                     \
      }                                                                       \
      while (0)

    #define SET(x) rot_tree_set(&tree, x, x, NULL)
    #define GET(x) rot_tree_get(&tree, x, NULL)
    #define REM(x) rot_tree_remove(&tree, x, NULL)

    // Empty tree
    SET_TREE();

    VERIFY_STRUCT(SET(1),
      1);
    VERIFY_STRUCT(GET(1));
    VERIFY_STRUCT(REM(1));

    END_TEST;

    // One-level tree
    SET_TREE(
      2);

    VERIFY_STRUCT(SET(2),
      2);
    VERIFY_STRUCT(SET(1),
       1,
      _,2);
    VERIFY_STRUCT(SET(3),
       3,
      2);
    VERIFY_STRUCT(GET(1),
      2);
    VERIFY_STRUCT(GET(2),
      2);
    VERIFY_STRUCT(REM(1),
      2);
    VERIFY_STRUCT(REM(2));

    END_TEST;

    // Two-level tree
    SET_TREE(
       4,
      2,6);

    VERIFY_STRUCT(SET(1),
         1,
       _,  4,
      _,_,2,6);
    VERIFY_STRUCT(SET(2),
        2,
      _,  4,
     _,_,_,6);
    VERIFY_STRUCT(SET(3),
         3,
       2,  4,
      _,_,_,6);
    VERIFY_STRUCT(SET(4),
       4,
      2,6);
    VERIFY_STRUCT(SET(5),
         5,
       4,  6,
      2,_,_,_);
    VERIFY_STRUCT(SET(6),
        6,
      4,  _,
     2,_,_,_);
    VERIFY_STRUCT(SET(7),
         7,
       4,  _,
      2,6,_,_);

    // Still using
    //  4
    // 2 6

    VERIFY_STRUCT(GET(1),
         2,
       _,  4,
      _,_,_,6);
    VERIFY_STRUCT(GET(2),
         2,
       _,  4,
      _,_,_,6);
    VERIFY_STRUCT(GET(3),
         2,
       _,  4,
      _,_,_,6);
    VERIFY_STRUCT(GET(4),
       4,
      2,6);
    VERIFY_STRUCT(GET(5),
         6,
       4,  _,
      2,_,_,_);
    VERIFY_STRUCT(GET(6),
         6,
       4,  _,
      2,_,_,_);
    VERIFY_STRUCT(GET(7),
         6,
       4,  _,
      2,_,_,_);

    // Still using
    //  4
    // 2 6

    VERIFY_STRUCT(REM(1),
         2,
       _,  4,
      _,_,_,6);
    VERIFY_STRUCT(REM(2),
       4,
      _,6);
    VERIFY_STRUCT(REM(3),
         2,
       _,  4,
      _,_,_,6);
    VERIFY_STRUCT(REM(4),
       2,
      _,6);
    VERIFY_STRUCT(REM(5),
         6,
       4,  _,
      2,_,_,_);
    VERIFY_STRUCT(REM(6),
       4,
      2);
    VERIFY_STRUCT(REM(7),
         6,
       4,  _,
      2,_,_,_);

    END_TEST;

    // Three-level tree
    SET_TREE(
         6,
       2,  10,
      0,4,8,12);

    // Root
    VERIFY_STRUCT(SET(6),
         6,
       2,  10,
      0,4,8,12);
    VERIFY_STRUCT(GET(6),
         6,
       2,  10,
      0,4,8,12);
    VERIFY_STRUCT(REM(6),
         4,
       2,  10,
      0,_,8,12);

    // Still using
    //    6
    //  2   10
    // 0 4 8 12

    // Middle level nodes
    VERIFY_STRUCT(SET(2),
             2,
         0,      6,
       _,  _,  4,  10,
      _,_,_,_,_,_,8,12);
    VERIFY_STRUCT(GET(2),
             2,
         0,      6,
       _,  _,  4,  10,
      _,_,_,_,_,_,8,12);
    VERIFY_STRUCT(REM(2),
         6,
       0,  10,
      _,4,8,12);
    VERIFY_STRUCT(SET(10),
             10,
         6,      12,
       2,  8,  _,  _,
      0,4,_,_,_,_,_,_);
    VERIFY_STRUCT(GET(10),
             10,
         6,      12,
       2,  8,  _,  _,
      0,4,_,_,_,_,_,_);
    VERIFY_STRUCT(REM(10),
         6,
       2,  8,
      0,4,_,12);

    // Still using
    //    6
    //  2   10
    // 0 4 8 12

    // Existing node on last level
    VERIFY_STRUCT(SET(4),
             4,
         2,      6,
       0,  _,  _,  10,
      _,_,_,_,_,_,8,12);
    VERIFY_STRUCT(GET(4),
             4,
         2,      6,
       0,  _,  _,  10,
      _,_,_,_,_,_,8,12);
    VERIFY_STRUCT(REM(4),
             2,
         0,      6,
       _,  _,  _,  10,
      _,_,_,_,_,_,8,12);

    // Still using
    //    6
    //  2   10
    // 0 4 8 12

    // Non-existing node on last level
    VERIFY_STRUCT(SET(5),
             5,
         2,      6,
       0,  4,  _,  10,
      _,_,_,_,_,_,8,12);
    VERIFY_STRUCT(GET(5),
             4,
         2,      6,
       0,  _,  _,  10,
      _,_,_,_,_,_,8,12);
    VERIFY_STRUCT(REM(5),
             4,
         2,      6,
       0,  _,  _,  10,
      _,_,_,_,_,_,8,12);

    END_TEST;

    #undef SET_TREE
    #undef VERIFY_STRUCT
    #undef END_TEST
    #undef SET
    #undef GET
    #undef REM
}

static void populate(Rot_tree *tree)
{
    #define ADD_DOUBLE(n)               \
      rot_tree_set(tree, n, 2*n, NULL); \
      VERIFY(rot_tree_valid(tree))

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

    rot_tree_set(tree, INT_MAX, 100, NULL);
    VERIFY(rot_tree_valid(tree));
    rot_tree_set(tree, INT_MIN, 200, NULL);
    VERIFY(rot_tree_valid(tree));

    #undef ADD_DOUBLE
}

static void test_set_get_helper(bool keys_exist)
{
    Rot_tree tree;

    rot_tree_init(&tree);
    if (keys_exist)
        populate(&tree);

    #define VERIFY_SET_GET(key, val)                      \
      {                                                   \
          int v = 234;                                    \
                                                          \
          if (!keys_exist) {                              \
              VERIFY(!rot_tree_get(&tree, key, &v));      \
              VERIFY(v == 234);                           \
              VERIFY(!rot_tree_set(&tree, key, val, &v)); \
              VERIFY(v == 234);                           \
          }                                               \
          VERIFY(rot_tree_get(&tree, key, &v));           \
          VERIFY(v == val);                               \
          v = 234;                                        \
          VERIFY(rot_tree_set(&tree, key, val + 1, &v));  \
          VERIFY(v == val);                               \
          VERIFY(rot_tree_get(&tree, key, &v));           \
          VERIFY(v == val + 1);                           \
          VERIFY(rot_tree_get(&tree, key, NULL));         \
          VERIFY(rot_tree_valid(&tree));                  \
      }

    for (int i = 0; i < 10; ++i)
        VERIFY_SET_GET(i, 2*i);
    VERIFY_SET_GET(INT_MAX, 100);
    VERIFY_SET_GET(INT_MIN, 200);

    #undef VERIFY_SET_GET

    rot_tree_free(&tree);
}

static void test_remove(void)
{
    Rot_tree tree;

    rot_tree_init(&tree);
    populate(&tree);

    #define VERIFY_REMOVE_NOT_EXISTS(key)           \
      {                                             \
          int v = 234;                              \
                                                    \
          VERIFY(!rot_tree_get(&tree, key, &v));    \
          VERIFY(!rot_tree_remove(&tree, key, &v)); \
          VERIFY(!rot_tree_get(&tree, key, &v));    \
          VERIFY(v == 234);                         \
      }

    #define VERIFY_REMOVE_EXISTS(key)                 \
      {                                               \
          int v1, v2;                                 \
                                                      \
          VERIFY(rot_tree_get(&tree, key, &v1));      \
          VERIFY(rot_tree_remove(&tree, key, &v2));   \
          VERIFY(!rot_tree_get(&tree, key, NULL));    \
          VERIFY(!rot_tree_remove(&tree, key, NULL)); \
          VERIFY(v1 == v2);                           \
      }

    VERIFY_REMOVE_NOT_EXISTS(10);
    VERIFY_REMOVE_NOT_EXISTS(-10);
    for (int i = 0; i < 10; ++i)
        VERIFY_REMOVE_EXISTS(i);
    VERIFY_REMOVE_EXISTS(INT_MIN);
    VERIFY_REMOVE_EXISTS(INT_MAX);

    #undef VERIFY_REMOVE_NOT_EXISTS
    #undef VERIFY_REMOVE_EXISTS

    rot_tree_free(&tree);
}

static void test_set_get(void)
{
    test_set_get_helper(false);
    test_set_get_helper(true);
}

void test_rot_tree(void)
{
    test_structure();
    test_set_get();
    test_remove();
}
