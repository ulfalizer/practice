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
    int *res = malloc(sizeof(int)*((argc == 0) ? 0 : argc - 1));
    for (int i = 1; i < argc; ++i) {
        char *end;
        res[i - 1] = strtol(argv[i], &end, 0);
        if (*argv[i] == '\0' || *end != '\0')
            fail("invalid integer argument '%s'", argv[i]);
    }
    return res;
}
