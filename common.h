#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void err(char const *format, ...)
  __attribute__((format(printf, 1, 2), noreturn));

void fail(char const *format, ...)
  __attribute__((format(printf, 1, 2), noreturn));

int *parse_int_args(int argc, char *argv[]);

void *emalloc(size_t size, const char *desc);
void *erealloc(void *ptr, size_t size, const char *desc);
char *estrdup(const char *s, const char *desc);

#define swap(a, b) do { typeof(a) tmp = a; a = b; b = tmp; } while(0)

#define max(a, b)         \
  ({ typeof(a) _a = a;    \
     typeof(b) _b = b;    \
     _a > _b ? _a : _b })

#define min(a, b)         \
  ({ typeof(a) _a = a;    \
     typeof(b) _b = b;    \
     _a < _b ? _a : _b })

#define STRINGIFY(x) STRINGIFY_(x)
#define STRINGIFY_(x) #x

#define VERIFY(cond)                                                  \
  if (!(cond))                                                        \
      fail(__FILE__":"STRINGIFY(__LINE__)": "#cond" should be true");

#define N_ARGS(...) N_ARGS_HELPER(dummy, ##__VA_ARGS__, \
  29, 28, 27, 26, 25, 24, 23, 22, 21, 20,               \
  19, 18, 17, 16, 15, 14, 13, 12, 11, 10,               \
   9,  8,  7,  6,  5,  4,  3,  2,  1,  0)
#define N_ARGS_HELPER(dummy,                        \
  n0 ,  n1,  n2,  n3,  n4,  n5,  n6,  n7,  n8, n9 , \
  n10, n11, n12, n13, n14, n15, n16, n17, n18, n19, \
  n20, n21, n22, n23, n24, n25, n26, n27, n28,   N, \
  ...) N
