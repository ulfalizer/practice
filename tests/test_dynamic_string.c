#include "common.h"
#include "dynamic_string.h"

#pragma GCC diagnostic ignored "-Wformat-security"

static void test_set() {
    String string;
    char buf[1024];

    string_init(&string);

    string_set(&string, "a%s%d%cl", "bcdefghij", 123, 'k');
    VERIFY(strcmp(string_get(&string), "abcdefghij123kl") == 0);

    for (size_t i = 0; i < ARRAY_LEN(buf); ++i) {
        for (size_t j = 0; j < i; ++j)
            buf[j] = (j % 32) + 1; // Arbitrary non-null values.
        buf[i] = '\0';

        string_set(&string, "%s", buf);
        VERIFY(string_len(&string) == i);
        VERIFY(strcmp(string_get(&string), buf) == 0);
        // Do some white-box testing too.
        VERIFY(string.buf_len >= ge_pow_2(i));
    }

    string_free(&string);
}

static void test_append_single_chars() {
    String string;
    char buf[2];

    string_init(&string);
    buf[1] = '\0';

    // a-z are strictly not guaranteed to be in this order, but in practice
    // they will be.

    // Append 15 characters (the limit just before the buffer is grown).
    for (buf[0] = 'a'; buf[0] <= 'o'; ++buf[0])
        string_append(&string, buf);

    VERIFY(strcmp(string_get(&string), "abcdefghijklmno") == 0);
    VERIFY(string_len(&string) == 15);
    // Do some white-box testing too.
    VERIFY(string.buf_len == 16);

    // Appending zero characters shouldn't change anything.
    buf[0] = '\0';
    string_append(&string, buf);
    VERIFY(strcmp(string_get(&string), "abcdefghijklmno") == 0);
    VERIFY(string_len(&string) == 15);
    VERIFY(string.buf_len == 16);

    // Appending one more character should grow the buffer.
    buf[0] = 'p';
    string_append(&string, buf);
    VERIFY(strcmp(string_get(&string), "abcdefghijklmnop") == 0);
    VERIFY(string_len(&string) == 16);
    VERIFY(string.buf_len == 32);

    string_free(&string);
}

static void test_append_short() {
    String string;

    string_init(&string);

    // Test conversion specifications. 15 characters written.
    string_append(&string, "a%s%d%cl", "bcdefghij", 123, 'k');
    VERIFY(strcmp(string_get(&string), "abcdefghij123kl") == 0);
    VERIFY(string_len(&string) == 15);
    VERIFY(string.buf_len == 16);

    // Add a few more characters so the buffer grows.
    string_append(&string, "%s%d", "foo", 100);
    VERIFY(strcmp(string_get(&string), "abcdefghij123klfoo100") == 0);
    VERIFY(string_len(&string) == 21);
    VERIFY(string.buf_len == 32);

    string_free(&string);
}

static void test_append_long() {
    String string;

    string_init(&string);

    // Add 32 characters (33 bytes with '\0'). This should immediately grow the
    // buffer size to 64.
    string_append(&string, "01234567890123456789012345678901");
    VERIFY(strcmp(string_get(&string),
      "01234567890123456789012345678901") == 0);
    VERIFY(string_len(&string) == 32);
    VERIFY(string.buf_len == 64);

    string_free(&string);
}

static void test_get_copy() {
    String string;
    char *copy;
    const char *s = "0123456789";

    string_init(&string);

    copy = string_get_copy(&string);
    VERIFY(*copy == '\0');
    free(copy);

    string_append(&string, s);
    copy = string_get_copy(&string);
    string_free(&string);
    VERIFY(strcmp(copy, s) == 0);

    free(copy);
}

void test_string() {
    test_set();
    test_append_single_chars();
    test_append_short();
    test_append_long();
    test_get_copy();
}
