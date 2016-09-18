// UTF-8-related functions

// Returns the number of characters in 's'. Assumes 's' is a well-formed
// null-terminated UTF-8 string.
size_t utf8_len(const char *s);

// Reverses the character sequence in 's' in-place. Assumes 's' is a
// well-formed null-terminated UTF-8 string.
void utf8_reverse(char *s);
