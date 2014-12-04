// Misc. functions and algorithms.

// Returns true if 'find' appears in 's'. Naive version.
bool substr(const char *find, const char *s);

// Returns the zero-based index of the 0 which, when replaced by a 1, yields
// the longest continuous sequence of 1s in 's'. 's' is assumed to consist of a
// sequence of '0' and '1' characters (where we use a character array for ease
// of testing).
//
// Returns the smallest index if there are many candidates and (size_t)-1 if
// 's' contains no 0s.
size_t max_ones(char *s);

typedef struct Vector Vector;

// Adds the common elements of 'a1' and 'a2' (of length 'a1_len' and 'a2_len',
// respectively) to 'res'. The values in each array are assumed to be in
// strictly ascending sorted order.
//
// Tries to skip over ranges of values in both arrays via binary search. The
// idea is that this might improve performance for inputs where the common
// elements are known to be few and far between. I haven't done any
// benchmarking though, and I suspect you'd need pretty special circumstances
// for this to be worthwhile.
//
// A simpler approach would be to keep a pointer for each list and always
// increment the one pointing to the smallest element. You could also
// experiment with different increments, etc.
void sorted_intersect(int *a1, size_t a1_len,
                      int *a2, size_t a2_len, Vector *res);

// Prints all strings with balanced left/right parentheses that have 'n'
// opening parentheses.
void print_balanced(int n);

// Returns true if 's' has balanced left/right parentheses from the set
// '(', '[', '{'. Assumes no other characters appear in 's'.
bool is_balanced(const char *s);

// Prints all permutations of characters in 's'.
void print_perms(char *s);

// Generates the permutations of 's' in-place using an iterative version of
// Heap's algorithm, calling 'fn' with each one. Generates ~763 million
// permutations per second with an empty 'fn' on a Core i7-2600K.
//
// Assumes strlen(s) >= 4 for simplicity.
void perm_heaps(char *s, void fn(char *perm));

// Generates the next higher permutation of 's' in lexicographic order. Returns
// false if no higher permutation exists.
bool next_lex(char *s);

// Returns true if 'find' appears in the sorted list 'nums' of length 'len'.
bool binsearch(int find, int *nums, size_t len);

// Prints 'num' in 'base'. Supports negative numbers and bases up to 36.
void print_num(int num, int base);

// Reverses the digits in 'num' when interpreted in base 'base'. Preserves
// sign.
int rev_num(int num, int base);
