// Misc. functions and algorithms.

bool substr(const char *find, const char *s);
// 's' is assumed to consist of a sequence of '0' and '1' characters (where we
// use a character array for ease of testing). Returns the zero-based index of
// the 0 which, when replaced by a 1, yields the longest continuous sequence of
// 1s in 's'.
//
// Returns the smallest index if there are many candidates and (size_t)-1 if
// 's' contains no 0s.
size_t max_ones(char *s);
// Prints all permutations of characters in 's'.
void print_perms(char *s);
// Generates the next higher permutation of 's' in lexicographic order. Returns
// false if no higher permutation exists.
bool next_lex(char *s);
bool binsearch(int find, int *nums, size_t len);
// Prints 'num' in 'base'. Supports negative numbers and bases up to 36.
void print_num(int num, int base);
// Reverses the digits in 'num' when interpreted in base 'base'. Preserves
// sign.
int rev_num(int num, int base);
// Generates the permutations of 's' in-place using an iterative version of
// Heap's algorithm, calling 'fn' with each one. Generates ~763 million
// permutations per second with an empty 'fn' on a Core i7-2600K.
//
// Assumes strlen(s) >= 4 for simplicity.
void perm_heaps(char *s, void fn(char *perm));
