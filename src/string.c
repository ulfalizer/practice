#include "common.h"
#include "string.h"

#define INITIAL_BUF_LEN 16

void string_init(String *s) {
    s->buf = emalloc(INITIAL_BUF_LEN, "string init");
    s->buf_len = INITIAL_BUF_LEN;
    s->len = 0;
    s->buf[0] = '\0';
}

void string_free(String *s) {
    free(s->buf);
}

// For ge_pow_2(), just to be pedantic.
_Static_assert(sizeof(unsigned long long) >= sizeof(size_t),
  "ge_pow_2() argument might overflow");

void string_append(String *s, const char *format, ...) {
    va_list ap, ap_copy;
    int addition_len;

    va_start(ap, format);
    // The first vsnprintf() will trash 'ap', so keep a copy in case we need to
    // repeat the operation.
    va_copy(ap_copy, ap);

    addition_len = vsnprintf(s->buf + s->len, s->buf_len - s->len, format, ap);
    if (s->len + addition_len >= s->buf_len) {
        // The resulting string won't fit in the buffer. Grow the buffer to the
        // next power of two that can accommodate it.
        s->buf_len = ge_pow_2(s->len + addition_len + 1);
        s->buf = erealloc(s->buf, s->buf_len, "string grow");

        // We should be good now. (Though maybe there's something extremely
        // obscure involving e.g. writing to an argument with %n as well as
        // using that value earlier on in the variable argument list that could
        // break this.)
        vsprintf(s->buf + s->len, format, ap_copy);
    }
    s->len += addition_len;

    va_end(ap_copy);
    va_end(ap);
}
