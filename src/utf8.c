#include "common.h"
#include "utf8.h"

size_t utf8_len(const char *s) {
    // Maps the initial byte of a multibyte character to the number of bytes in
    // the encoding.
    static const uint8_t char_to_len[256] =
      { [0xC0 ... 0xDF] = 2,
        [0xE0 ... 0xEF] = 3,
        [0xF0 ... 0xF7] = 4 };
    size_t len = 0;
    // The signedness of 'char' is implementation-defined. This lets us safely
    // compare against 0 to check properties.
    signed char *ss = (signed char*)s;

    do {
        // Count spans of single-byte characters and spans of multibyte
        // characters separately to speed things up on common texts.

        // Count span of single-byte characters.
        for (; *ss > 0; ++len, ++ss);

        // Count span of multibyte characters.
        for (; *ss < 0; ++len)
            ss += char_to_len[(uc)*ss];
    }
    while (*ss != '\0');

    return len;
}
