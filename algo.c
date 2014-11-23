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
