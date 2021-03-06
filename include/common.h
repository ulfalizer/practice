// Common system headers, helper functions, and macros.

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <setjmp.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

// Prints a message together with errno (with a newline appended) to stderr and
// exits with EXIT_FAILURE
noreturn void err_exit(const char *format, ...)
  __attribute__((format(printf, 1, 2)));

// Prints a message to stderr (with a newline appended) and exits with
// EXIT_FAILURE
noreturn void fail_exit(const char *format, ...)
  __attribute__((format(printf, 1, 2)));

// Returns a list of integers parsed from 'argv'. Caller frees list.
int *parse_int_args(int argc, char *argv[]);

// These functions print 'desc' and exit with EXIT_FAILURE if the allocation
// fails
void *emalloc(size_t size, const char *desc);
void *emalloc_align(size_t size, size_t align, const char *desc);
void *erealloc(void *ptr, size_t size, const char *desc);
char *estrdup(const char *s, const char *desc);

typedef unsigned char uc;

#define swap(a, b) do { typeof(a) tmp = a; a = b; b = tmp; } while(0)

#define max(a, b)         \
  ({ typeof(a) _a = a;    \
     typeof(b) _b = b;    \
     _a > _b ? _a : _b; })

#define min(a, b)         \
  ({ typeof(a) _a = a;    \
     typeof(b) _b = b;    \
     _a < _b ? _a : _b; })

#define uninitialized_var(x) x = x

#define UNUSED __attribute__((unused))

#define ARRAY_LEN(a) (sizeof (a)/sizeof *(a))

// Returns its argument as a string literal
#define STRINGIFY(x) STRINGIFY_(x)
#define STRINGIFY_(x) #x

#ifdef NDEBUG
#  define UNREACHABLE __builtin_unreachable()
#else
#  define UNREACHABLE assert(!"reached \"unreachable\" code")
#endif

// Optimization hints. All uses have been shown to decrease code size with GCC.
#define assume(x)      \
  do                   \
      if (!(x))        \
          UNREACHABLE; \
  while (0)

#define VERIFY(cond)                                                       \
  if (!(cond))                                                             \
      fail_exit(__FILE__":"STRINGIFY(__LINE__)": "#cond" should be true");

// Returns the number of arguments in the variable argument list. Supports zero
// arguments via a GCC extension.
#define N_ARGS(...) N_ARGS_HELPER(dummy, ##__VA_ARGS__, \
  29, 28, 27, 26, 25, 24, 23, 22, 21, 20,               \
  19, 18, 17, 16, 15, 14, 13, 12, 11, 10,               \
   9,  8,  7,  6,  5,  4,  3,  2,  1,  0)
#define N_ARGS_HELPER(dummy,                        \
  n0 ,  n1,  n2,  n3,  n4,  n5,  n6,  n7,  n8, n9 , \
  n10, n11, n12, n13, n14, n15, n16, n17, n18, n19, \
  n20, n21, n22, n23, n24, n25, n26, n27, n28,   N, \
  ...) N

// Returns the least power of two greater than or equal to 'n', except for
// returning 0 for n = 0 (which makes sense given "doubling" semantics)
unsigned long long ge_pow_2(unsigned long long n);
