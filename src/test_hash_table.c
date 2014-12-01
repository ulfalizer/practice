#include "common.h"
#include "hash_table.h"

static void populate(Hash_table *hash_table) {
    hash_table_set(hash_table, "negative one", -1, NULL);
    hash_table_set(hash_table, "zero", 0, NULL);
    hash_table_set(hash_table, "one", 1, NULL);
    hash_table_set(hash_table, "two", 2, NULL);
    hash_table_set(hash_table, "three", 3, NULL);
    hash_table_set(hash_table, "four", 4, NULL);
    hash_table_set(hash_table, "five", 5, NULL);
    hash_table_set(hash_table, "six", 6, NULL);
    hash_table_set(hash_table, "seven", 7, NULL);
    hash_table_set(hash_table, "eight", 8, NULL);
    hash_table_set(hash_table, "nine", 9, NULL);
    hash_table_set(hash_table, "ten", 10, NULL);
    hash_table_set(hash_table, "", 123, NULL);
}

static void test_set_get_helper(bool keys_exist) {
    #define VERIFY_SET_GET(key, val)                              \
      {                                                           \
          int v = 234;                                            \
          if (!keys_exist) {                                      \
              VERIFY(!hash_table_get(&hash_table, key, &v));      \
              VERIFY(v == 234);                                   \
              VERIFY(!hash_table_set(&hash_table, key, val, &v)); \
              VERIFY(v == 234);                                   \
          }                                                       \
          VERIFY(hash_table_get(&hash_table, key, &v));           \
          VERIFY(v == val);                                       \
          v = 234;                                                \
          VERIFY(hash_table_set(&hash_table, key, val + 1, &v));  \
          VERIFY(v == val);                                       \
          VERIFY(hash_table_get(&hash_table, key, &v));           \
          VERIFY(v == val + 1);                                   \
          VERIFY(hash_table_get(&hash_table, key, NULL));         \
      }

    Hash_table hash_table;
    hash_table_init(&hash_table);
    if (keys_exist)
        populate(&hash_table);
    VERIFY_SET_GET("ten", 10);
    VERIFY_SET_GET("negative one", -1);
    VERIFY_SET_GET("nine", 9);
    VERIFY_SET_GET("zero", 0);
    VERIFY_SET_GET("eight", 8);
    VERIFY_SET_GET("one", 1);
    VERIFY_SET_GET("seven", 7);
    VERIFY_SET_GET("two", 2);
    VERIFY_SET_GET("six", 6);
    VERIFY_SET_GET("three", 3);
    VERIFY_SET_GET("five", 5);
    VERIFY_SET_GET("four", 4);
    VERIFY_SET_GET("", 123);
    hash_table_free(&hash_table);

    #undef VERIFY_SET_GET
}

static void test_set_get() {
    test_set_get_helper(false);
    test_set_get_helper(true);
}

static void test_remove() {
    #define VERIFY_REMOVE_NOT_EXISTS(key)                   \
      {                                                     \
          int v = 234;                                      \
          VERIFY(!hash_table_get(&hash_table, key, &v));    \
          VERIFY(!hash_table_remove(&hash_table, key, &v)); \
          VERIFY(!hash_table_get(&hash_table, key, &v));    \
          VERIFY(v == 234);                                 \
      }

    #define VERIFY_REMOVE_EXISTS(key)                         \
      {                                                       \
          int v1, v2;                                         \
          VERIFY(hash_table_get(&hash_table, key, &v1));      \
          VERIFY(hash_table_remove(&hash_table, key, &v2));   \
          VERIFY(!hash_table_get(&hash_table, key, NULL));    \
          VERIFY(!hash_table_remove(&hash_table, key, NULL)); \
          VERIFY(v1 == v2);                                   \
      }

    Hash_table hash_table;
    hash_table_init(&hash_table);
    populate(&hash_table);
    VERIFY_REMOVE_NOT_EXISTS("foo");
    VERIFY_REMOVE_NOT_EXISTS("on");
    VERIFY_REMOVE_NOT_EXISTS("tw");
    VERIFY_REMOVE_EXISTS("negative one");
    VERIFY_REMOVE_EXISTS("zero");
    VERIFY_REMOVE_EXISTS("one");
    VERIFY_REMOVE_EXISTS("two");
    VERIFY_REMOVE_EXISTS("three");
    VERIFY_REMOVE_EXISTS("four");
    VERIFY_REMOVE_EXISTS("five");
    VERIFY_REMOVE_EXISTS("six");
    VERIFY_REMOVE_EXISTS("seven");
    VERIFY_REMOVE_EXISTS("eight");
    VERIFY_REMOVE_EXISTS("nine");
    VERIFY_REMOVE_EXISTS("ten");
    VERIFY_REMOVE_EXISTS("");

    // The case where the key exists and val is NULL.
    hash_table_set(&hash_table, "foo", 3, NULL);
    VERIFY(hash_table_remove(&hash_table, "foo", NULL));

    hash_table_free(&hash_table);

    #undef VERIFY_REMOVE_NOT_EXISTS
    #undef VERIFY_REMOVE_EXISTS
}

static const char *gen_key(int i) {
    static char buf[32];
    sprintf(buf, "%d", i);
    return buf;
}

static void test_resize() {
    Hash_table hash_table;

    hash_table_init(&hash_table);
    for (int i = 0; i < 512; ++i) {
        for (int j = 0; j < i; ++j) {
            int n;
            hash_table_get(&hash_table, gen_key(j), &n);
            VERIFY(n == j);
        }
        hash_table_set(&hash_table, gen_key(i), i, NULL);
    }
    hash_table_free(&hash_table);
}

void test_hash_table() {
    test_set_get();
    test_remove();
    test_resize();
}
