#include "common.h"
#include "utf8.h"

static void test_len() {
    // ONE, TWO, etc. are the smallest code points that get encoded with that
    // many bytes (skipping the null character for the ONE case). <N>_MAX are
    // the largest code points encoded with <N> bytes.

    // We are not allowed to use universal character names for the first three
    // characters, so specify the raw encodings instead.
    #define ONE       "\x01"
    #define ONE_MAX   "\x7F"
    #define TWO       "\xC2\x80"
    #define TWO_MAX   u8"\u07FF"
    #define THREE     u8"\u0800"
    #define THREE_MAX u8"\uFFFF"
    #define FOUR      u8"\U00010000"
    #define FOUR_MAX  u8"\U001FFFFF"

    static_assert(sizeof ONE       == 2, "expected one-byte encoding");
    static_assert(sizeof ONE_MAX   == 2, "expected one-byte encoding");
    static_assert(sizeof TWO       == 3, "expected two-byte encoding");
    static_assert(sizeof TWO_MAX   == 3, "expected two-byte encoding");
    static_assert(sizeof THREE     == 4, "expected three-byte encoding");
    static_assert(sizeof THREE_MAX == 4, "expected three-byte encoding");
    static_assert(sizeof FOUR      == 5, "expected four-byte encoding");
    static_assert(sizeof FOUR_MAX  == 5, "expected four-byte encoding");

    #define L(str, len) VERIFY(utf8_len(str) == len)

    L("", 0);
    L(ONE, 1);
    L(ONE_MAX, 1);
    L(TWO, 1);
    L(TWO_MAX, 1);
    L(THREE, 1);
    L(THREE_MAX, 1);
    L(FOUR, 1);
    L(FOUR_MAX, 1);

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

    #undef ONE
    #undef ONE_MAX
    #undef TWO
    #undef TWO_MAX
    #undef THREE
    #undef THREE_MAX
    #undef FOUR
    #undef FOUR_MAX
}

static void test_reverse() {
    // Various characters with three-byte UTF-8 encodings.
    #define T1 u8"\u0800"
    #define T2 u8"\u0911"
    #define T3 u8"\u0A22"
    #define T4 u8"\u0B33"
    #define T5 u8"\u0C44"

    // Various characters with four-byte UTF-8 encodings.
    #define F1 u8"\U00010000"
    #define F2 u8"\U00111111"
    #define F3 u8"\U00144444"
    #define F4 u8"\U00199999"
    #define F5 u8"\U001CCCCC"

    #define VERIFY_REV(str1, str2)     \
      {                                \
          char s1[] = str1;            \
          const char *s2 = str2;       \
                                       \
          utf8_reverse(s1);            \
          VERIFY(strcmp(s1, s2) == 0); \
      }

    // One-byte encodings.
    VERIFY_REV(u8"", u8"");
    VERIFY_REV(u8"a", u8"a");
    VERIFY_REV(u8"ab", u8"ba");
    VERIFY_REV(u8"abc", u8"cba");
    VERIFY_REV(u8"abcd", u8"dcba");
    VERIFY_REV(u8"abcde", u8"edcba");

    // Two-byte encodings.
    VERIFY_REV(u8"å", u8"å");
    VERIFY_REV(u8"åä", u8"äå");
    VERIFY_REV(u8"åäö", u8"öäå");
    VERIFY_REV(u8"åäöÅ", u8"Åöäå");
    VERIFY_REV(u8"åäöÅÄ", u8"ÄÅöäå");

    // Three-byte encodings.
    VERIFY_REV(T1, T1);
    VERIFY_REV(T1 T2, T2 T1);
    VERIFY_REV(T1 T2 T3, T3 T2 T1);
    VERIFY_REV(T1 T2 T3 T4, T4 T3 T2 T1);
    VERIFY_REV(T1 T2 T3 T4 T5, T5 T4 T3 T2 T1);

    // Four-byte encodings.
    VERIFY_REV(F1, F1);
    VERIFY_REV(F1 F2, F2 F1);
    VERIFY_REV(F1 F2 F3, F3 F2 F1);
    VERIFY_REV(F1 F2 F3 F4, F4 F3 F2 F1);
    VERIFY_REV(F1 F2 F3 F4 F5, F5 F4 F3 F2 F1);

    // Mixed-length encodings.
    VERIFY_REV(u8"aå", u8"åa");
    VERIFY_REV(u8"abåä", u8"äåba");
    VERIFY_REV(u8"aå" T1 F1, F1 T1 u8"åa");
    VERIFY_REV(T1 F1 u8"åa", u8"aå" F1 T1);

    #undef VERIFY_REV

    #undef T1
    #undef T2
    #undef T3
    #undef T4
    #undef T5

    #undef F1
    #undef F2
    #undef F3
    #undef F4
    #undef F5
}

void test_utf8() {
    test_len();
    test_reverse();
}
