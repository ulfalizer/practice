// Linux kernel-style linked list with node pointers embedded directly in the
// list elements, which saves memory (no data pointers) and improves data
// locality by storing pointers and data together. The drawback is
// harder-to-understand code.
//
// The list is doubly-linked and circular and has a separate 'head' pair of
// pointers (with no corresponding data) where the 'next' pointer points to the
// first element of the list and the 'prev' pointer to the last.
//
// A practical implementation would add more functionality, of course. :)
//
// This is not strict-aliasing safe.

// Structs that want to be part of a linked list include this
typedef struct List_node {
    struct List_node *next;
    struct List_node *prev;
} List_node;

// Assuming 'ptr' points to the member named 'member' within an instance of
// 'container', returns a pointer to the container.
//
// The point of the 'member_' assignment is to generate a warning if the type
// of 'ptr' is incompatible with the type of the member. We also use 'member_'
// instead of 'ptr' in the address calculation to avoid an unused variable
// warning.
#define container_of(ptr, member, container) ({                  \
  const typeof(((container*)0)->member) *member_ = (ptr);        \
  (container*)((char*)member_ - offsetof(container, member)); })

// Helper macro for iterating over a list. Assigns each list element (of type
// 'container') in turn to 'cur', which can be accessed in the body.
//
// The kernel version is more general (doesn't assume the member is named
// "list_node" and allows the loop cursor to use to be specified).
#define list_for_each(head, container)                                    \
  for (container *cur = container_of((head)->next, list_node, container); \
    &cur->list_node != (head);                                            \
    cur = container_of(cur->list_node.next, list_node, container))

// Like list_for_each, but iterates backwards
#define list_for_each_backwards(head, container)                          \
  for (container *cur = container_of((head)->prev, list_node, container); \
    &cur->list_node != (head);                                            \
    cur = container_of(cur->list_node.prev, list_node, container))
