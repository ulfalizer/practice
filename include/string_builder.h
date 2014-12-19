// A dynamically grown string builder.

typedef struct String {
    char *buf;
    size_t buf_len;
    size_t len;
} String;

void string_init(String *s);
void string_free(String *s);

// Appends text to 's'. Format is like for printf().
void string_append(String *s, const char *format, ...)
  __attribute__((format(printf, 2, 3)));

// Returns a pointer to the string. The string is null-terminated.
//
// The pointer is only guaranteed to remain valid until the next append
// operation, and becomes invalid when the buffer is freed.
char *string_get(String *s);

// Safer version of string_get(). Returns a copy of the string, which is
// free()d by the caller.
char *string_get_copy(String *s);

// Returns the length of the string (excluding the terminating null).
size_t string_len(String *s);
