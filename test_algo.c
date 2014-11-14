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

void test_algo() {
    test_substr();
    test_binsearch();
}
