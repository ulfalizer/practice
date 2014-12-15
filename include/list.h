// Unidirectional linked list.

typedef struct Node {
    struct Node *next;
    int val;
} Node;

typedef struct List {
    struct Node *start;
} List;

void list_init(List *list);
void list_free(List *list);

// Creates a list with the 'len' ints from the variable argument list.
void list_make(List *list, size_t len, ...);

// Adds 'val' to the beginning of 'list'.
void list_add(List *list, int val);

// Returns true if 'list' equals the 'len' int values from the variable
// argument list.
bool list_equals(List *list, size_t len, ...);

// Returns true if lists 'l1' and 'l2' are equal.
bool lists_equal(List *l1, List *l2);

// Returns true if the values in 'list' are monotonically increasing.
bool list_is_sorted(List* list);

// Removes the first instance of 'val' from 'list'.
void list_remove(List *list, int val);

// Removes all instances of 'val' from 'list'.
void list_remove_all(List *list, int val);

// Reverses 'list'.
void list_reverse(List *list);

// Sort functions.
void list_selection_sort(List *list);
void list_insertion_sort(List *list);
void list_mergesort(List *list);

// Prints 'list'.
void list_print(List *list);
