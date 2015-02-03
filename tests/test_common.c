#include "common.h"

static void test_ge_pow_2(void) {
    VERIFY(ge_pow_2(1) == 2);
    for (unsigned long i = 2; i <= 1025; ++i)
        VERIFY(ge_pow_2(i) == 2 << (int)log2(i - 1));
}

void test_common(void) {
    test_ge_pow_2();
}
