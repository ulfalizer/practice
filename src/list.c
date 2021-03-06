#include "common.h"
#include "list.h"

void list_init(List *list)
{
    list->start = NULL;
}

void list_free(List *list)
{
    Node *next;

    for (Node *node = list->start; node; node = next) {
        next = node->next;
        free(node);
    }
}

void list_make(List *list, size_t len, ...)
{
    va_list ap;
    Node **cur;

    va_start(ap, len);
    for (cur = &list->start; len > 0; cur = &(*cur)->next, --len) {
        *cur = emalloc(sizeof **cur, "list make");
        (*cur)->val = va_arg(ap, int);
    }
    va_end(ap);

    *cur = NULL;
}

void list_add(List *list, int val)
{
    Node *new_node = emalloc(sizeof *new_node, "list add");

    new_node->next = list->start;
    new_node->val = val;
    list->start = new_node;
}

bool list_equals(List *list, size_t len, ...)
{
    va_list ap;
    Node *node;

    va_start(ap, len);
    for (node = list->start; node && len > 0; node = node->next, --len)
        if (node->val != va_arg(ap, int)) {
            va_end(ap);

            return false;
        }
    va_end(ap);

    return node == NULL && len == 0;
}

bool lists_equal(List *l1, List *l2)
{
    for (Node *n1 = l1->start, *n2 = l2->start;;
         n1 = n1->next, n2 = n2->next) {
        if (n1 == NULL)
            return n2 == NULL;
        if (n2 == NULL)
            return n1 == NULL;
        if (n1->val != n2->val)
            return false;
    }
}

bool list_is_sorted(List* list)
{
    int prev = INT_MIN;

    for (Node *node = list->start; node; prev = node->val, node = node->next)
        if (prev > node->val)
            return false;

    return true;
}

void list_remove(List *list, int val)
{
    for (Node **cur = &list->start; *cur; cur = &(*cur)->next)
        if ((*cur)->val == val) {
            Node *tmp = *cur;

            *cur = (*cur)->next;
            free(tmp);

            return;
        }
}

void list_remove_all(List *list, int val)
{
    for (Node **cur = &list->start; *cur;)
        if ((*cur)->val == val) {
            Node *tmp = *cur;

            *cur = (*cur)->next;
            free(tmp);
        }
        else
            cur = &(*cur)->next;
}

void list_reverse(List *list)
{
    Node *next, *prev = NULL;

    for (Node *cur = list->start; cur; prev = cur, cur = next) {
        next = cur->next;
        cur->next = prev;
    }
    list->start = prev;
}

// Helper function - removes and returns the minimum elements from the list.
// Assumes there is at least one element
static Node *extract_min(Node **node)
{
    Node **min = node, *res;

    // Locate the minimum element
    for (; *node; node = &(*node)->next)
        if ((*node)->val < (*min)->val)
            min = node;
    res = *min;
    // Update the pointer to the minimum element to point to the following
    // element instead
    *min = (*min)->next;

    return res;
}

// Assumed correct and used to test other sorting algorithms
void list_selection_sort(List *list)
{
    for (Node **node = &list->start; *node; node = &(*node)->next) {
        Node *min = extract_min(node);
        min->next = *node;
        *node = min;
    }
}

// Helper function - inserts 'node' into 'sorted', which is sorted in ascending
// order, in a way that preserves sortedness
static void list_insert(Node *node, Node **sorted)
{
    Node **cur;

    // Find the pointer to the first element whose value is greater than or
    // equal to the node's, if any (otherwise we'll get the final (NULL)
    // pointer).
    for (cur = sorted; *cur && (*cur)->val < node->val; cur = &(*cur)->next);
    // Insert 'node' before that element.
    node->next = *cur;
    *cur = node;
}

// Most efficient when the reverse of the list is almost sorted. Not stable.
void list_insertion_sort(List *list)
{
    Node *cur = list->start, *next;

    // list->start points to the sorted list we insert into. It's initially
    // empty.
    list->start = NULL;
    for (; cur; cur = next) {
        next = cur->next;
        list_insert(cur, &list->start);
    }
}

// Merges the (ascendingly) sorted lists 'first' and 'second' into one sorted
// list. Updates 'first' to point to the resulting list.
static void merge(Node **first, Node *second)
{
    for (Node **cur = first;; cur = &(*cur)->next) {
        if (*cur == NULL) {
            // No elements remain in 'first'. Append the rest of 'second' to
            // it.
            *cur = second;

            return;
        }
        if (second == NULL)
            return;
        if (second->val < (*cur)->val)
            // 'second' should be the next element, so assign it to *cur.
            // Simultaneously make the old continuation of 'cur' the new
            // 'second' so we don't lose it.
            swap(*cur, second);
    }
}

// Splits the list started by 'node' into two lists such that the second list
// gets floor(#elements/2) elements. Returns the second list.
//
// This would be faster if we explicitly stored the length of lists.
static Node *split(Node *node)
{
    Node *speedy = node, *tmp;

    if (node == NULL)
        return NULL;
    for (;;) {
        if ((speedy = speedy->next) == NULL)
            break;
        if ((speedy = speedy->next) == NULL)
            break;
        node = node->next;
    }
    tmp = node->next;
    // Terminate the first list at the midpoint.
    node->next = NULL;

    return tmp;
}

static void mergesort_rec(Node **node)
{
    Node *other = split(*node);

    if (other == NULL)
        return;

    mergesort_rec(node);
    mergesort_rec(&other);
    merge(node, other);
}

void list_mergesort(List *list)
{
    mergesort_rec(&list->start);
}

void list_print(List *list)
{
    // Avoid printing an extra space after the last element
    if (list->start) {
        printf("%d", list->start->val);
        for (Node *node = list->start->next; node; node = node->next)
            printf(" %d", node->val);
    }
    putchar('\n');
}
