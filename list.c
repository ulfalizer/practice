#include "common.h"
#include "list.h"

void list_add(Node **node, int val) {
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL)
        err("malloc new_node");
    new_node->next = *node;
    new_node->val = val;
    *node = new_node;
}

Node *list_make(size_t len, ...) {
    va_list ap;
    void list_reverse(Node **node);
    Node *node = NULL;

    va_start(ap, len);
    while (len--)
        list_add(&node, va_arg(ap, int));
    va_end(ap);
    list_reverse(&node);
    return node;
}

bool list_equals(Node *node, size_t len, ...) {
    va_list ap;
    va_start(ap, len);
    for (; node && len > 0; node = node->next, --len)
        if (node->val != va_arg(ap, int)) {
            va_end(ap);
            return false;
        }
    va_end(ap);
    return node == NULL && len == 0;
}

bool lists_equal(Node *l1, Node *l2) {
    for (;; l1 = l1->next, l2 = l2->next) {
        if (l1 == NULL)
            return l2 == NULL;
        if (l2 == NULL)
            return l1 == NULL;
        if (l1->val != l2->val)
            return false;
    }
}

bool list_is_sorted(Node* node) {
    for (int prev = INT_MIN; node; prev = node->val, node = node->next)
        if (prev > node->val)
            return false;
    return true;
}

void list_remove(Node **node, int val) {
    for (Node **cur = node; *cur; cur = &(*cur)->next)
        if ((*cur)->val == val) {
            Node *tmp = *cur;
            *cur = (*cur)->next;
            free(tmp);
            return;
        }
}

void list_remove_all(Node **node, int val) {
    for (Node **cur = node; *cur;)
        if ((*cur)->val == val) {
            Node *tmp = *cur;
            *cur = (*cur)->next;
            free(tmp);
        }
        else
            cur = &(*cur)->next;
}

void list_reverse(Node **node) {
    Node *next, *prev = NULL;
    for (Node *cur = *node; cur; prev = cur, cur = next) {
        next = cur->next;
        cur->next = prev;
    }
    *node = prev;
}

static Node *extract_min(Node **node) {
    Node **min = node, *res;
    for (; *node; node = &(*node)->next)
        if ((*node)->val < (*min)->val)
            min = node;
    res = *min;
    *min = (*min)->next;
    return res;
}

// Assumed correct and used to test other sorting algorithms.
void list_selection_sort(Node **node) {
    for (; *node; node = &(*node)->next) {
        Node *min = extract_min(node);
        min->next = *node;
        *node = min;
    }
}

static void list_insert(Node **sorted, Node *node) {
    Node **cur;
    for (cur = sorted; *cur && (*cur)->val < node->val; cur = &(*cur)->next);
    node->next = *cur;
    *cur = node;
}

// Most efficient when the reverse of the list is almost sorted. Not stable.
void list_insertion_sort(Node **node) {
    Node *cur;
    if (*node == NULL)
        return;
    cur = (*node)->next;
    (*node)->next = NULL;
    while (cur) {
        Node *next = cur->next;
        list_insert(node, cur);
        cur = next;
    }
}

void list_merge(Node **first, Node *second) {
    for (Node **cur = first;; cur = &(*cur)->next) {
        if (*cur == NULL) {
            *cur = second;
            return;
        }
        if (second == NULL)
            return;
        if ((*cur)->val >= second->val)
            swap(*cur, second);
    }
}

Node *list_split(Node *node) {
    Node *speedy = node;
    if (node == NULL)
        return NULL;
    for (;;) {
        if ((speedy = speedy->next) == NULL)
            break;
        if ((speedy = speedy->next) == NULL)
            break;
        node = node->next;
    }
    Node *tmp = node->next;
    node->next = NULL;
    return tmp;
}

void list_mergesort(Node **node) {
    Node *other = list_split(*node);
    if (other == NULL)
        return;
    list_mergesort(node);
    list_mergesort(&other);
    list_merge(node, other);
}

void list_print(Node *node) {
    if (node) {
        printf("%d", node->val);
        for (node = node->next; node; node = node->next)
            printf(" %d", node->val);
    }
    putchar('\n');
}

void list_free(Node *node) {
    Node *next;
    for (; node; node = next) {
        next = node->next;
        free(node);
    }
}
