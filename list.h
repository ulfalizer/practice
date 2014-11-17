typedef struct Node {
    struct Node *next;
    int val;
} Node;

typedef struct List {
    struct Node *start;
} List;

void list_init(List *list);
void list_free(List *list);
void list_make(List *list, size_t len, ...);
void list_add(List *list, int val);
bool list_equals(List *list, size_t len, ...);
bool lists_equal(List *l1, List *l2);
bool list_is_sorted(List* list);
void list_remove(List *list, int val);
void list_remove_all(List *list, int val);
void list_reverse(List *list);
void list_selection_sort(List *list);
void list_insertion_sort(List *list);
void list_mergesort(List *list);
void list_print(List *list);
