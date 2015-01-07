// A dynamically grown string. Reuses its internal buffer whenever possible.
//
// The non-raw operations null-terminate. The raw operations do not. They can
// be safely combined in an intuitive way. The combination of raw and non-raw
// operations enables flexible and efficient use as a buffer.
//
// Don't call the file "string.h" since it collides with the libc header.

typedef struct String {
    char *buf;
    size_t buf_len;
    // The length of the string, excluding the terminating null for non-raw
    // operations.
    size_t len;
} String;

// Initializes 's'. It is initially empty.
void string_init(String *s);
void string_free(String *s);

// Sets the contents of 's'. Format is like for printf(). The result is
// null-terminated.
void string_set(String *s, const char *format, ...)
  __attribute__((format(printf, 2, 3)));

// Equivalent to string_set() but takes a va_list instead of a variable number
// of arguments.
void string_set_v(String *s, const char *format, va_list ap)
  __attribute__((format(printf, 2, 0)));

// Sets the contents to 's' to 'data', of length 'len'.
//
// Supports null bytes in the input. The result is _not_ null-terminated, but
// this function can still be safely combined with all other functions since an
// explicit count is used internally to keep track of the length.
void string_set_raw(String *s, const char *data, size_t len);

// Appends text to 's'. Format is like for printf().
//
// If the previous operation was non-raw, the first byte will overwrite the
// terminating null byte as expected. Format is like for printf(). The result
// is null-terminated.
void string_append(String *s, const char *format, ...)
  __attribute__((format(printf, 2, 3)));

// Equivalent to string_append() but takes a va_list instead of a variable
// number of arguments.
void string_append_v(String *s, const char *format, va_list ap)
  __attribute__((format(printf, 2, 0)));

// Appends 'data' of length 'len' to 's'. If the previous operation was
// non-raw, the first byte will overwrite the terminating null byte.
//
// Supports null bytes in the input. The result is _not_ null-terminated, but
// this function can still be safely combined with all other functions since an
// explicit count is used internally to keep track of the length.
void string_append_raw(String *s, const char *data, size_t len);

// Returns a pointer to the string.
//
// The pointer is only guaranteed to remain valid until the next append
// operation, and becomes invalid when the buffer is freed.
char *string_get(String *s);

// Returns the length of the string (excluding the terminating null byte if the
// previous operation was non-raw).
size_t string_len(String *s);
