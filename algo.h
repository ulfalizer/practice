// Misc. functions and algorithms.

bool substr(const char *find, const char *s);
// Prints all permutations of characters in 's'
void print_perms(char *s);
bool binsearch(int find, int *nums, size_t len);
// Prints 'num' in 'base'. Supports negative numbers and bases up to 36.
void print_num(int num, int base);
// Reverses the digits in 'num' when interpreted in base 'base'. Preserves
// sign.
int rev_num(int num, int base);
