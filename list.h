// TODO: Clean up to hide list representation

typedef struct Node {
    struct Node *next;
    int val;
} Node;

void list_add(Node **node, int val);
Node *list_make(size_t len, ...);
bool list_equals(Node *node, size_t len, ...);
bool lists_equal(Node *l1, Node *l2);
bool list_is_sorted(Node* node);
void list_remove(Node **node, int val);
void list_remove_all(Node **node, int val);
void list_free(Node *node);
void list_reverse(Node **node);
void list_selection_sort(Node **node);
void list_insertion_sort(Node **node);
void list_merge(Node **first, Node *second);
Node *list_split(Node *node);
void list_mergesort(Node **node);
void list_print(Node *node);
