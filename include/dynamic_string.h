// A dynamically grown string. Don't call the file "string.h" since it collides
// with the libc header.

typedef struct String {
    char *buf;
    size_t buf_len;
    // The length of the string, excluding the terminating null
    size_t len;
} String;

// Initializes 's'. It is initially empty.
void string_init(String *s);
void string_free(String *s);

// Sets the contents of 's'. Format is like for printf().
//
// This function reuses the internal buffer and grows it only as necessary.
void string_set(String *s, const char *format, ...)
  __attribute__((format(printf, 2, 3)));

// Equivalent to string_set(), but takes a va_list instead of a variable number
// of arguments
void string_set_v(String *s, const char *format, va_list ap)
  __attribute__((format(printf, 2, 0)));

// Appends text to 's'. Format is like for printf().
void string_append(String *s, const char *format, ...)
  __attribute__((format(printf, 2, 3)));

// Equivalent to string_append(), but takes a va_list instead of a variable
// number of arguments
void string_append_v(String *s, const char *format, va_list ap)
  __attribute__((format(printf, 2, 0)));

// Returns a pointer to the string. The string is null-terminated.
//
// The pointer is only guaranteed to remain valid until the next append
// operation, and becomes invalid when the buffer is freed.
char *string_get(String *s);

// Safer version of string_get(). Returns a copy of the string, which is
// free()d by the caller.
char *string_get_copy(String *s);

// Returns the length of the string (excluding the terminating null byte)
size_t string_len(String *s);
