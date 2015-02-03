#include "common.h"
#include "algo.h"
#include "vector.h"

static void test_substr(void) {
    VERIFY(substr("", ""));
    VERIFY(substr("", "a"));
    VERIFY(!substr("a", ""));
    VERIFY(substr("a", "a"));
    VERIFY(substr("a", "ba"));
    VERIFY(substr("a", "ab"));
    VERIFY(!substr("foo", ""));
    VERIFY(!substr("foo", "f"));
    VERIFY(!substr("foo", "fo"));
    VERIFY(!substr("foo", "fobar"));
    VERIFY(substr("foo", "foo"));
    VERIFY(substr("foo", "xxfoo"));
    VERIFY(substr("foo", "fooxx"));
    VERIFY(substr("foo", "xxfooxx"));
}

static void test_max_ones(void) {
    VERIFY(max_ones("") == (size_t)-1);
    VERIFY(max_ones("0") == 0);
    VERIFY(max_ones("1") == (size_t)-1);
    VERIFY(max_ones("00") == 0);
    VERIFY(max_ones("01") == 0);
    VERIFY(max_ones("10") == 1);
    VERIFY(max_ones("11") == (size_t)-1);
    VERIFY(max_ones("000") == 0);
    VERIFY(max_ones("001") == 1);
    VERIFY(max_ones("010") == 0);
    VERIFY(max_ones("011") == 0);
    VERIFY(max_ones("100") == 1);
    VERIFY(max_ones("101") == 1);
    VERIFY(max_ones("110") == 2);
    VERIFY(max_ones("111") == (size_t)-1);
    VERIFY(max_ones("000000") == 0);
    VERIFY(max_ones("000001") == 4);
    VERIFY(max_ones("000010") == 3);
    VERIFY(max_ones("000011") == 3);
    VERIFY(max_ones("000100") == 2);
    VERIFY(max_ones("000101") == 4);
    VERIFY(max_ones("000110") == 2);
    VERIFY(max_ones("000111") == 2);
    VERIFY(max_ones("001000") == 1);
    VERIFY(max_ones("001001") == 1);
    VERIFY(max_ones("001010") == 3);
    VERIFY(max_ones("001011") == 3);
    VERIFY(max_ones("001100") == 1);
    VERIFY(max_ones("001101") == 4);
    VERIFY(max_ones("001110") == 1);
    VERIFY(max_ones("001111") == 1);
    VERIFY(max_ones("010000") == 0);
    VERIFY(max_ones("010001") == 0);
    VERIFY(max_ones("010010") == 0);
    VERIFY(max_ones("010011") == 3);
    VERIFY(max_ones("010100") == 2);
    VERIFY(max_ones("010101") == 2);
    VERIFY(max_ones("010110") == 2);
    VERIFY(max_ones("010111") == 2);
    VERIFY(max_ones("011000") == 0);
    VERIFY(max_ones("011001") == 0);
    VERIFY(max_ones("011010") == 3);
    VERIFY(max_ones("011011") == 3);
    VERIFY(max_ones("011100") == 0);
    VERIFY(max_ones("011101") == 4);
    VERIFY(max_ones("011110") == 0);
    VERIFY(max_ones("011111") == 0);
    VERIFY(max_ones("100000") == 1);
    VERIFY(max_ones("100001") == 1);
    VERIFY(max_ones("100010") == 1);
    VERIFY(max_ones("100011") == 3);
    VERIFY(max_ones("100100") == 1);
    VERIFY(max_ones("100101") == 4);
    VERIFY(max_ones("100110") == 2);
    VERIFY(max_ones("100111") == 2);
    VERIFY(max_ones("101000") == 1);
    VERIFY(max_ones("101001") == 1);
    VERIFY(max_ones("101010") == 1);
    VERIFY(max_ones("101011") == 3);
    VERIFY(max_ones("101100") == 1);
    VERIFY(max_ones("101101") == 1);
    VERIFY(max_ones("101110") == 1);
    VERIFY(max_ones("101111") == 1);
    VERIFY(max_ones("110000") == 2);
    VERIFY(max_ones("110001") == 2);
    VERIFY(max_ones("110010") == 2);
    VERIFY(max_ones("110011") == 2);
    VERIFY(max_ones("110100") == 2);
    VERIFY(max_ones("110101") == 2);
    VERIFY(max_ones("110110") == 2);
    VERIFY(max_ones("110111") == 2);
    VERIFY(max_ones("111000") == 3);
    VERIFY(max_ones("111001") == 3);
    VERIFY(max_ones("111010") == 3);
    VERIFY(max_ones("111011") == 3);
    VERIFY(max_ones("111100") == 4);
    VERIFY(max_ones("111101") == 4);
    VERIFY(max_ones("111110") == 5);
    VERIFY(max_ones("111111") == (size_t)-1);
    VERIFY(max_ones("0000100000000") == 3);
    VERIFY(max_ones("0000100110000") == 6);
    VERIFY(max_ones("1100100110011") == 2);
    VERIFY(max_ones("1100101110011") == 5);
    VERIFY(max_ones("1100101110111") == 9);
}

static void verify_vector_equals_helper(Vector *v, size_t len, ...) {
    va_list ap;

    VERIFY(vector_len(v) == len);

    va_start(ap, len);
    for (size_t i = 0; i < len; ++i)
        VERIFY((intptr_t)vector_get(v, i) == va_arg(ap, int));
    va_end(ap);
}

#define VERIFY_VECTOR_EQUALS(v, ...) \
  verify_vector_equals_helper(&v, N_ARGS(__VA_ARGS__), ##__VA_ARGS__)

static void test_sorted_intersect(void) {
    #define ARRAY_1(...)                                                \
      do {                                                              \
          int a1[] = { __VA_ARGS__ }
    #define ARRAY_2(...)                                                \
          int a2[] = { __VA_ARGS__ }
    #define VERIFY_COMMON(...)                                          \
          Vector res;                                                   \
                                                                        \
          vector_init(&res);                                            \
          sorted_intersect(a1, ARRAY_LEN(a1), a2, ARRAY_LEN(a2), &res); \
          VERIFY_VECTOR_EQUALS(res, ##__VA_ARGS__);                     \
          vector_free(&res);                                            \
      }                                                                 \
      while (0)

    ARRAY_1();
    ARRAY_2();
    VERIFY_COMMON();

    ARRAY_1(1);
    ARRAY_2();
    VERIFY_COMMON();

    ARRAY_1();
    ARRAY_2(1);
    VERIFY_COMMON();

    ARRAY_1(1);
    ARRAY_2(   2);
    VERIFY_COMMON();

    ARRAY_1(1);
    ARRAY_2(1);
    VERIFY_COMMON(1);

    ARRAY_1(1, 2);
    ARRAY_2(1);
    VERIFY_COMMON(1);

    ARRAY_1(1, 2);
    ARRAY_2(   2);
    VERIFY_COMMON(2);

    ARRAY_1(1);
    ARRAY_2(1, 2);
    VERIFY_COMMON(1);

    ARRAY_1(   2);
    ARRAY_2(1, 2);
    VERIFY_COMMON(2);

    ARRAY_1(   1,    3);
    ARRAY_2(0,    2);
    VERIFY_COMMON();

    ARRAY_1(0,    2);
    ARRAY_2(   1,    3);
    VERIFY_COMMON();

    ARRAY_1(0, 1, 2, 3, 4, 5);
    ARRAY_2(0, 1, 2, 3, 4, 5);
    VERIFY_COMMON(0, 1, 2, 3, 4, 5);

    ARRAY_1(0, 1, 2, 3, 4   );
    ARRAY_2(   1, 2, 3, 4, 5);
    VERIFY_COMMON(1, 2, 3, 4);

    ARRAY_1(   1, 2, 3, 4, 5);
    ARRAY_2(0, 1, 2, 3, 4   );
    VERIFY_COMMON(1, 2, 3, 4);

    ARRAY_1(0, 1, 2,    4, 5, 6,    8, 9,     11,             15);
    ARRAY_2(      2, 3, 4,       7,    9, 10,     12, 13, 14, 15, 16, 17);
    VERIFY_COMMON(2, 4, 9, 15);

    ARRAY_1(      2, 3, 4,       7,    9, 10,     12, 13, 14, 15, 16, 17);
    ARRAY_2(0, 1, 2,    4, 5, 6,    8, 9,     11,             15);
    VERIFY_COMMON(2, 4, 9, 15);

    #undef ARRAY_1
    #undef ARRAY_2
    #undef VERIFY_COMMON
}

static void test_is_balanced(void) {
    VERIFY(is_balanced(""));
    VERIFY(is_balanced("()"));
    VERIFY(is_balanced("{}"));
    VERIFY(is_balanced("[]"));
    VERIFY(is_balanced("()()"));
    VERIFY(is_balanced("(){}"));
    VERIFY(is_balanced("()[]"));
    VERIFY(is_balanced("(())"));
    VERIFY(is_balanced("[{}]"));
    VERIFY(is_balanced("{[]}"));
    VERIFY(is_balanced("[]({[][]}[]){()}"));

    VERIFY(!is_balanced("("));
    VERIFY(!is_balanced("{"));
    VERIFY(!is_balanced("["));
    VERIFY(!is_balanced(")"));
    VERIFY(!is_balanced("}"));
    VERIFY(!is_balanced("]"));
    VERIFY(!is_balanced(")("));
    VERIFY(!is_balanced("()("));
    VERIFY(!is_balanced("())"));
    VERIFY(!is_balanced("(()"));
    VERIFY(!is_balanced("(}"));
    VERIFY(!is_balanced("(]"));
    VERIFY(!is_balanced("{)"));
    VERIFY(!is_balanced("[)"));
    VERIFY(!is_balanced("[())"));
    VERIFY(!is_balanced("([))"));
    VERIFY(!is_balanced("((])"));
    VERIFY(!is_balanced("(()]"));
}

static void test_binsearch(void) {
    #define VERIFY_BINSEARCH(in_list, find, ...)                    \
      {                                                             \
        int a[] = { __VA_ARGS__ };                                  \
                                                                    \
        VERIFY(in_list == binsearch(find, a, N_ARGS(__VA_ARGS__))); \
      }

    VERIFY_BINSEARCH(false, 1);
    VERIFY_BINSEARCH(false, 1,  2);
    VERIFY_BINSEARCH(false, 2,  1);
    VERIFY_BINSEARCH(false, 0,  1, 3);
    VERIFY_BINSEARCH(false, 2,  1, 3);
    VERIFY_BINSEARCH(false, 4,  1, 3);
    VERIFY_BINSEARCH(false, 0,  1, 3, 5);
    VERIFY_BINSEARCH(false, 2,  1, 3, 5);
    VERIFY_BINSEARCH(false, 4,  1, 3, 5);
    VERIFY_BINSEARCH(false, 6,  1, 3, 5);
    VERIFY_BINSEARCH(false, 0,  1, 3, 5, 7);
    VERIFY_BINSEARCH(false, 2,  1, 3, 5, 7);
    VERIFY_BINSEARCH(false, 4,  1, 3, 5, 7);
    VERIFY_BINSEARCH(false, 6,  1, 3, 5, 7);
    VERIFY_BINSEARCH(false, 8,  1, 3, 5, 7);

    VERIFY_BINSEARCH(true, 1,  1);
    VERIFY_BINSEARCH(true, 1,  1, 2);
    VERIFY_BINSEARCH(true, 2,  1, 2);
    VERIFY_BINSEARCH(true, 1,  1, 2, 3);
    VERIFY_BINSEARCH(true, 2,  1, 2, 3);
    VERIFY_BINSEARCH(true, 3,  1, 2, 3);
    VERIFY_BINSEARCH(true, 1,  1, 2, 3, 4);
    VERIFY_BINSEARCH(true, 2,  1, 2, 3, 4);
    VERIFY_BINSEARCH(true, 3,  1, 2, 3, 4);
    VERIFY_BINSEARCH(true, 4,  1, 2, 3, 4);
    VERIFY_BINSEARCH(true, 1,  1, 2, 3, 4, 5);
    VERIFY_BINSEARCH(true, 2,  1, 2, 3, 4, 5);
    VERIFY_BINSEARCH(true, 3,  1, 2, 3, 4, 5);
    VERIFY_BINSEARCH(true, 4,  1, 2, 3, 4, 5);
    VERIFY_BINSEARCH(true, 5,  1, 2, 3, 4, 5);

    #undef VERIFY_BINSEARCH
}

static void test_next_lex(void) {
    #define TEST_STRING(str)          \
      do {                            \
          char s[] = str              \

    #define VERIFY_NEXT_PERM(str)     \
          VERIFY(next_lex(s));        \
          VERIFY(strcmp(s, str) == 0)
    #define VERIFY_NO_MORE_PERMS      \
          VERIFY(!next_lex(s))
    #define END_TEST                  \
      }                               \
      while (0)

    TEST_STRING("");
    VERIFY_NO_MORE_PERMS;
    END_TEST;

    TEST_STRING("a");
    VERIFY_NO_MORE_PERMS;
    END_TEST;

    TEST_STRING("ab");
    VERIFY_NEXT_PERM("ba");
    VERIFY_NO_MORE_PERMS;
    END_TEST;

    TEST_STRING("abc");
    VERIFY_NEXT_PERM("acb");
    VERIFY_NEXT_PERM("bac");
    VERIFY_NEXT_PERM("bca");
    VERIFY_NEXT_PERM("cab");
    VERIFY_NEXT_PERM("cba");
    VERIFY_NO_MORE_PERMS;
    END_TEST;

    TEST_STRING("aa");
    VERIFY_NO_MORE_PERMS;
    END_TEST;

    TEST_STRING("aab");
    VERIFY_NEXT_PERM("aba");
    VERIFY_NEXT_PERM("baa");
    VERIFY_NO_MORE_PERMS;
    END_TEST;

    TEST_STRING("abb");
    VERIFY_NEXT_PERM("bab");
    VERIFY_NEXT_PERM("bba");
    VERIFY_NO_MORE_PERMS;
    END_TEST;

    TEST_STRING("aabb");
    VERIFY_NEXT_PERM("abab");
    VERIFY_NEXT_PERM("abba");
    VERIFY_NEXT_PERM("baab");
    VERIFY_NEXT_PERM("baba");
    VERIFY_NEXT_PERM("bbaa");
    VERIFY_NO_MORE_PERMS;
    END_TEST;

    #undef TEST_STRING
    #undef VERIFY_NEXT_PERM
    #undef VERIFY_NO_MORE_PERMS
    #undef END_TEST
}

void test_algo(void) {
    test_substr();
    test_max_ones();
    test_sorted_intersect();
    test_is_balanced();
    test_binsearch();
    test_next_lex();
}
