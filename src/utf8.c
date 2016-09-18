#include "common.h"
#include "utf8.h"

size_t utf8_len(const char *s)
{
    // Maps the initial byte of a multibyte character to the number of bytes in
    // the encoding
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
        // characters separately to speed things up on common texts

        // Count span of single-byte characters
        for (; *ss > 0; ++len, ss += 1);

        // Count span of multibyte characters
        for (; *ss < 0; ++len, ss += char_to_len[(uc)*ss]);
    }
    while (*ss != '\0');

    return len;
}

static void reverse(char *start, char *end)
{
    for (; start < end; ++start, --end)
        swap(*start, *end);
}

void utf8_reverse(char *s)
{
    char *start = s;

    // Reverse the bytes in each character in the string
    while (*s != '\0') {
        char *first = s;

        // Skip past any continuation bytes
        while ((*++s & 0xC0) == 0x80);

        // Reverse the character's bytes
        reverse(first, s - 1);
    }

    // Reverse the bytes in the entire string. We save a strlen() by reusing
    // the length we indirectly calculated above.
    reverse(start, s - 1);
}
