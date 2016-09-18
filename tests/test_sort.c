#include "common.h"
#include "sort.h"

static void test_sort_fn(void sort_fn(int *nums, size_t n_nums))
{
    // Assume selection sort is correct and test other sorting algorithms by
    // comparing against it
    #define VERIFY_SORT(...)                                  \
      {                                                       \
          int a1[] = { __VA_ARGS__ }, a2[] = { __VA_ARGS__ }; \
                                                              \
          selection_sort(a1, N_ARGS(__VA_ARGS__));            \
          sort_fn(a2, N_ARGS(__VA_ARGS__));                   \
          for (size_t i = 0; i < N_ARGS(__VA_ARGS__); ++i)    \
              VERIFY(a1[i] == a2[i]);                         \
      }

    #pragma GCC diagnostic push // Avoid clang warning
    #pragma GCC diagnostic ignored "-Wtype-limits"
    VERIFY_SORT();
    #pragma GCC diagnostic pop
    VERIFY_SORT(1);
    VERIFY_SORT(1, 1);
    VERIFY_SORT(1, 2);
    VERIFY_SORT(2, 1);
    VERIFY_SORT(1, 1, 1);
    VERIFY_SORT(1, 2, 3);
    VERIFY_SORT(1, 3, 2);
    VERIFY_SORT(2, 1, 3);
    VERIFY_SORT(2, 3, 1);
    VERIFY_SORT(3, 1, 2);
    VERIFY_SORT(3, 2, 1);
    VERIFY_SORT(1, 2, 3, 4);
    VERIFY_SORT(8, 1, 7, 2, -5, 6, 3, 5, 4, 1, 1, 2);

    #undef VERIFY_SORT
}

void test_sort(void)
{
    test_sort_fn(insertion_sort);
    test_sort_fn(bubble_sort);
    test_sort_fn(mergesort);
    test_sort_fn(quicksort);
    test_sort_fn(heapsort);
}
