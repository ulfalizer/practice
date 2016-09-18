#include "common.h"
#include "embedded_list.h"
#include "vector.h"

typedef struct Data_node {
    int val;
    // Put the links last as it exercises container_of() better
    List_node list_node;
} Data_node;

// Turns 'nodes' into a circular list with 'head' holding the head pointers.
// Uses 0, 1, 2, ..., 'len' for values.
static void init_nodes(List_node *head, Data_node *nodes, size_t len)
{
    head->next = &nodes[0].list_node;
    head->prev = &nodes[len - 1].list_node;

    for (size_t i = 0; i < len; ++i) {
        nodes[i].val = i;

        if (i == 0)
            nodes[i].list_node.prev = head;
        else
            nodes[i].list_node.prev = &nodes[i - 1].list_node;

        if (i == len - 1)
            nodes[i].list_node.next = head;
        else
            nodes[i].list_node.next = &nodes[i + 1].list_node;
    }
}

static void verify_vector_equals_helper(Vector *v, size_t len, ...)
{
    va_list ap;

    VERIFY(vector_len(v) == len);

    va_start(ap, len);
    for (size_t i = 0; i < len; ++i)
        VERIFY((intptr_t)vector_get(v, i) == va_arg(ap, int));
    va_end(ap);
}

#define VERIFY_VECTOR_EQUALS(v, ...) \
  verify_vector_equals_helper(&v, N_ARGS(__VA_ARGS__), ##__VA_ARGS__)

void test_embedded_list(void)
{
    Data_node data_nodes[10];
    List_node head;
    Vector res;

    vector_init(&res);

    init_nodes(&head, data_nodes, ARRAY_LEN(data_nodes));

    list_for_each(&head, Data_node)
        vector_append(&res, (void*)(intptr_t)cur->val);
    list_for_each_backwards(&head, Data_node)
        vector_append(&res, (void*)(intptr_t)cur->val);

    VERIFY_VECTOR_EQUALS(res, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
      9, 8, 7, 6, 5, 4, 3, 2, 1, 0);

    vector_free(&res);
}
