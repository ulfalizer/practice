#include "common.h"
#include "sort.h"

// Assumed correct and used to test other sorting algorithms.
void selection_sort(int *nums, size_t n) {
    if (n == 0)
        return;

    for (size_t i = 0; i < n - 1; ++i) {
        size_t j, min_index = i;
        for (j = i + 1; j < n; ++j)
            if (nums[j] < nums[min_index])
                min_index = j;
        swap(nums[i], nums[min_index]);
    }
}

void bubble_sort(int *nums, size_t n) {
    bool sorted;

    if (n == 0)
        return;

    do {
        sorted = true;
        for (size_t i = 0; i < n - 1; ++i)
            if (nums[i] > nums[i + 1]) {
                sorted = false;
                swap(nums[i], nums[i + 1]);
            }
    }
    while (!sorted);
}

void insertion_sort(int *nums, size_t n) {
    for (size_t i = 1; i < n; ++i) {
        int cur = nums[i];
        size_t j;
        for (j = i; j > 0 && nums[j - 1] > cur; --j)
            nums[j] = nums[j - 1];
        nums[j] = cur;
    }
}

static void mergesort_rec(int *nums, size_t n, int *scratch) {
    size_t i1, i2, mid;

    if (n <= 1)
        return;

    i1 = 0;
    i2 = mid = n/2;
    mergesort_rec(nums, mid, scratch);
    mergesort_rec(nums + mid, n - mid, scratch);

    // Merge.
    for (size_t i = 0;; ++i) {
        if (i1 == mid) {
            memcpy(scratch + i, nums + i2, sizeof(int)*(n - i2));
            break;
        }
        if (i2 == n) {
            memcpy(scratch + i, nums + i1, sizeof(int)*(mid - i1));
            break;
        }
        scratch[i] = (nums[i1] < nums[i2]) ? nums[i1++] : nums[i2++];
    }
    memcpy(nums, scratch, sizeof(int)*n);
}

void mergesort(int *nums, size_t n) {
    int *scratch = emalloc(sizeof(int)*n, "mergesort scratch");

    mergesort_rec(nums, n, scratch);
    free(scratch);
}

void quicksort(int *nums, size_t n) {
    size_t store_i;

    if (n <= 1)
        return;

    // Partition using the last element as the pivot.
    store_i = 0;
    for (size_t i = 0; i < n - 1; ++i)
        if (nums[i] < nums[n - 1]) {
            swap(nums[store_i], nums[i]);
            ++store_i;
        }
    swap(nums[store_i], nums[n - 1]);

    // Possible optimization:
    // Always making the call on the largest partition the tail-recursive call
    // would guarantee log(n) space.
    quicksort(nums, store_i);
    quicksort(nums + store_i + 1, n - store_i - 1);
}

static size_t parent(size_t i) { return (i - 1)/2; }
static size_t left_child(size_t i) { return 2*i + 1; }
static size_t right_child(size_t i) { return 2*i + 2; }

static void trickle_down(int *nums, size_t p, size_t n) {
    while (left_child(p) < n) {
        size_t max_index = p;

        if (nums[left_child(p)] > nums[max_index])
            max_index = left_child(p);
        if (right_child(p) < n && nums[right_child(p)] > nums[max_index])
            max_index = right_child(p);

        if (max_index == p)
            return;

        swap(nums[p], nums[max_index]);
        p = max_index;
    }
}

static void heapify(int *nums, size_t n) {
    size_t p = parent(n - 1);

    do
        trickle_down(nums, p, n);
    while (p-- != 0);
}

void heapsort(int *nums, size_t n) {
    size_t i;

    if (n <= 1)
        return;

    heapify(nums, n);

    i = n - 1;
    do {
        swap(nums[0], nums[i]);
        trickle_down(nums, 0, i);
    }
    while (i-- != 0);
}
