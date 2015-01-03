#include "common.h"
#include "utf8.h"

// Characters with varying encoding lengths.
#define ONE   u8"$"
#define TWO   u8"¢"
#define THREE u8"€"
#define FOUR  u8"𐍈"

static_assert(sizeof ONE == 2, "expected one-byte UTF-8 encoding");
static_assert(sizeof TWO == 3, "expected two-byte UTF-8 encoding");
static_assert(sizeof THREE == 4, "expected three-byte UTF-8 encoding");
static_assert(sizeof FOUR == 5, "expected four-byte UTF-8 encoding");

static void test_len() {
    #define L(str, len) VERIFY(utf8_len(str) == len)

    L("", 0);

    L(ONE, 1);
    L(TWO, 1);
    L(THREE, 1);
    L(FOUR, 1);

    L(ONE ONE, 2);
    L(ONE TWO, 2);
    L(ONE THREE, 2);
    L(ONE FOUR, 2);
    L(TWO ONE, 2);
    L(TWO TWO, 2);
    L(TWO THREE, 2);
    L(TWO FOUR, 2);
    L(THREE ONE, 2);
    L(THREE TWO, 2);
    L(THREE THREE, 2);
    L(THREE FOUR, 2);
    L(FOUR ONE, 2);
    L(FOUR TWO, 2);
    L(FOUR THREE, 2);
    L(FOUR FOUR, 2);

    L(ONE ONE ONE ONE ONE, 5);
    L(TWO ONE ONE ONE ONE, 5);
    L(ONE TWO ONE ONE ONE, 5);
    L(TWO ONE TWO ONE ONE, 5);
    L(TWO ONE ONE TWO ONE, 5);
    L(TWO ONE ONE ONE TWO, 5);

    L(TWO TWO TWO TWO TWO, 5);
    L(ONE TWO TWO TWO TWO, 5);
    L(TWO ONE TWO TWO TWO, 5);
    L(TWO TWO ONE TWO TWO, 5);
    L(TWO TWO TWO ONE TWO, 5);
    L(TWO TWO TWO TWO ONE, 5);

    #undef L
}

void test_utf8() {
    test_len();
}
