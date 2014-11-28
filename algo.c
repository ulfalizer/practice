#include "common.h"
#include "algo.h"

bool substr(const char *find, const char *s) {
    for (size_t i = 0; s[i] != '\0'; ++i) {
        for (size_t j = 0; find[j] != '\0'; ++j)
            if (s[i + j] != find[j])
                goto no_match_at_i;
        return true;
        no_match_at_i: ;
    }
    return *find == '\0';
}

size_t max_ones(char *s) {
    size_t i;
    size_t max = 0;
    size_t uninitialized_var(max_i);
    size_t prev_zero = -1;
    size_t prev_prev_zero = -1;

    for (i = 0; s[i] != '\0'; ++i)
        if (s[i] == '0') {
            if (i - prev_prev_zero > max) {
                max = i - prev_prev_zero;
                max_i = prev_zero;
            }
            prev_prev_zero = prev_zero;
            prev_zero = i;
        }

    return (i - prev_prev_zero > max) ? prev_zero : max_i;
}

static void print_balanced_rec(int i, int remain, int level, char *res) {
    // No opening parentheses remain to be placed. Place remaining closing
    // parentheses and return result.
    if (remain == 0) {
        while (res[i] != '\0')
            res[i++] = ')';
        puts(res);

        return;
    }

    // Place left parenthesis and recurse.
    res[i] = '(';
    print_balanced_rec(i + 1, remain - 1, level + 1, res);
    // Place right parenthesis and recurse, if possible.
    if (level > 0) {
        res[i] = ')';
        print_balanced_rec(i + 1, remain, level - 1, res);
    }
}

void print_balanced(int n) {
    char *res;

    if (n == 0)
        return;

    res = alloca(2*n + 1);
    memset(res, 1, 2*n);
    res[2*n] = '\0';
    print_balanced_rec(0, n, 0, res);
}

static void print_perms_rec(char *s, size_t i) {
    if (s[i] == '\0')
        puts(s);
    for (size_t j = i; s[j] != '\0'; ++j) {
        swap(s[i], s[j]);
        print_perms_rec(s, i + 1);
        swap(s[i], s[j]);
    }
}

void print_perms(char *s) {
    if (*s == '\0')
        return;
    print_perms_rec(s, 0);
}

bool next_lex(char *s) {
    size_t i, swap_i;
    size_t len;

    if (*s == '\0')
        return false;
    len = strlen(s);
    // Go left while elements are monotonically increasing.
    for (i = len - 1; i > 0 && (uc)s[i - 1] >= (uc)s[i]; --i);
    if (i == 0)
        // Already lexicographically sorted.
        return false;
    swap_i = i - 1;
    // Find the smallest element greater than s[swap_i] on its right and swap
    // it for s[swap_i]. No need to check for the end of the string since we
    // will terminate on null anyway.
    for (++i; (uc)s[i] > (uc)s[swap_i]; ++i);
    swap(s[swap_i], s[i - 1]);
    // Reverse the sequence on the right of swap_i.
    for (i = 0; i < (len - swap_i)/2; ++i)
        swap(s[swap_i + 1 + i], s[len - 1 - i]);
    return true;
}

// Unrolled base case for generating all permutations of four elements. Speeds
// things up by a little less than 2x compared to no base case on my machine.
// Base case with three elements is ~15% slower than with four elements. (All
// tests run with an empty 'fn'.)
static void permute_4(char *s, void fn(char *perm)) {
    char a = s[0], b = s[1], c = s[2], d = s[3];
    fn(s); s[0] = b; s[1] = a; fn(s); s[0] = c; s[2] = b;
    fn(s); s[0] = a; s[1] = c; fn(s); s[0] = b; s[2] = a;
    fn(s); s[0] = c; s[1] = b; fn(s); s[0] = d; s[3] = c;
    fn(s); s[0] = b; s[1] = d; fn(s); s[0] = a; s[2] = b;
    fn(s); s[0] = d; s[1] = a; fn(s); s[0] = b; s[2] = d;
    fn(s); s[0] = a; s[1] = b; fn(s); s[1] = c; s[3] = b;
    fn(s); s[0] = c; s[1] = a; fn(s); s[0] = d; s[2] = c;
    fn(s); s[0] = a; s[1] = d; fn(s); s[0] = c; s[2] = a;
    fn(s); s[0] = d; s[1] = c; fn(s); s[2] = b; s[3] = a;
    fn(s); s[0] = c; s[1] = d; fn(s); s[0] = b; s[2] = c;
    fn(s); s[0] = d; s[1] = b; fn(s); s[0] = c; s[2] = d;
    fn(s); s[0] = b; s[1] = c; fn(s);
}

void perm_heaps(char *s, void fn(char *perm)) {
    // The current level. The valid indices at level 'lev' are
    // 0, 1, ..., lev.
    size_t lev;
    // indices[lev] holds the current index for level 'lev'.
    size_t *indices;
    // Length of string to permute.
    size_t len;

    len = strlen(s);
    assert(len >= 4);
    indices = alloca(sizeof(size_t)*len);
    for (size_t i = 0; i < len; ++i)
        indices[i] = 0;

    permute_4(s, fn);
    for (lev = 4; lev < len;)
        if (indices[lev] < lev) {
            // Select an element and swap it for the last element using Heap's
            // strategy.
            size_t swap_pos = (lev & 1) ? indices[lev] : 0;
            swap(s[swap_pos], s[lev]);
            // Increase the index at this level.
            ++indices[lev];
            // "Recurse" all the way down to the unrolled base case.
            lev = 4;
            permute_4(s, fn);
        }
        else
            // We're done at this level. Set the index to zero for when (if) we
            // return to this level later, and then "return" to the next higher
            // level.
            indices[lev++] = 0;
}

bool binsearch(int find, int *nums, size_t len) {
    // The search range is [left,right[.
    size_t left = 0, middle, right = len;
    while (left != right) {
        middle = left + (right - left)/2; // Overflow safe.
        if (nums[middle] > find)
            right = middle;
        else if (nums[middle] < find)
            left = middle + 1;
        else
            return true;
    }
    return false;
}

void print_num(int num, int base) {
    // Enough space to hold the digits of INT_MIN in binary plus a minus sign
    // and a terminating null.
    #define MAX_SIZE (1 + CHAR_BIT*sizeof(num) + 1)
    static const char digits[] =
      { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y', 'Z' };
    char buf[MAX_SIZE];
    int i;
    bool negative;

    negative = num < 0;
    buf[MAX_SIZE - 1] = '\0';
    i = MAX_SIZE - 1;
    do {
        // Handles negative numbers in a way that supports INT_MIN (which has
        // no representable inverse).
        buf[--i] = digits[abs(num%base)];
        num /= base;
    }
    while (num != 0);
    if (negative)
        buf[--i] = '-';
    puts(buf + i);
    #undef MAX_SIZE
}

int rev_num(int num, int base) {
    int res = 0;

    while (num != 0) {
        res = base*res + num%base;
        num /= base;
    }
    return res;
}
