// Common helper functions

#include "common.h"

static void common_fail(bool print_errno, char const *format, va_list ap)
  __attribute__((noreturn));

static void common_fail(bool print_errno, char const *format, va_list ap) {
    vfprintf(stderr, format, ap);
    if (print_errno)
        fprintf(stderr, ": %s", strerror(errno));
    putc('\n', stderr);
    exit(EXIT_FAILURE);
}

void err(char const *format, ...) {
    va_list ap;
    va_start(ap, format);
    common_fail(true, format, ap);
}

void fail(char const *format, ...) {
    va_list ap;
    va_start(ap, format);
    common_fail(false, format, ap);
}

int *parse_int_args(int argc, char *argv[]) {
    void *emalloc(size_t size, const char *desc);
    int *res = emalloc(sizeof(int)*((argc == 0) ? 0 : argc - 1), "args");
    for (int i = 1; i < argc; ++i) {
        char *end;
        res[i - 1] = strtol(argv[i], &end, 0);
        if (*argv[i] == '\0' || *end != '\0')
            fail("invalid integer argument '%s'", argv[i]);
    }
    return res;
}

void *emalloc(size_t size, const char *desc) {
    void *res = malloc(size);
    if (res == NULL)
        err("malloc failed: %s", desc);
    return res;
}

void *erealloc(void *ptr, size_t size, const char *desc) {
    void *res = realloc(ptr, size);
    if (res == NULL)
        err("realloc failed: %s", desc);
    return res;
}

char *estrdup(const char *s, const char *desc) {
    char *res = strdup(s);
    if (res == NULL)
        err("strdup failed: %s", desc);
    return res;
}
