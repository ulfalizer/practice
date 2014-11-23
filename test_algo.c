#include "common.h"
#include "algo.h"

static void test_substr() {
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

static void test_binsearch() {
    #define VERIFY_BINSEARCH(in_list, find, ...)                    \
      {                                                             \
        int a[] = { __VA_ARGS__ };                                  \
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

static void test_next_lex() {
    #define TEST_STRING(str) \
      do {                   \
          char s[] = str
    #define VERIFY_NEXT_PERM(str)      \
          VERIFY(next_lex(s));         \
          VERIFY(strcmp(s, str) == 0)
    #define VERIFY_NO_MORE_PERMS \
          VERIFY(!next_lex(s))
    #define END_TEST \
      }              \
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

void test_algo() {
    test_substr();
    test_binsearch();
    test_next_lex();
}
