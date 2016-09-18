#include "common.h"
#include "search_tree.h"
#include "tree.h"

void search_tree_init(Search_tree *tree)
{
    tree->root = NULL;
}

void search_tree_free(Search_tree *tree)
{
    tree_free(tree->root);
}

// Returns the address of the pointer to the node holding 'key'. If 'key' does
// not exist in the tree, returns the address of the (NULL) pointer that could
// be updated to insert it.
static Tree_node **get_node_ptr(Search_tree *tree, int key)
{
    Tree_node **cur = &tree->root;

    while (*cur && key != (*cur)->key)
        cur = (key < (*cur)->key) ? &(*cur)->left : &(*cur)->right;

    return cur;
}

bool search_tree_set(Search_tree *tree, int key, int val, int *old_val)
{
    Tree_node **node = get_node_ptr(tree, key);

    if (*node == NULL) {
        *node = create_node(key, val, NULL, NULL);

        return false;
    }

    if (old_val != NULL)
        *old_val = (*node)->val;
    (*node)->val = val;

    return true;
}

bool search_tree_get(Search_tree *tree, int key, int *val)
{
    Tree_node *node = *get_node_ptr(tree, key);

    if (node == NULL)
        return false;

    if (val != NULL)
        *val = node->val;

    return true;
}

bool search_tree_remove(Search_tree *tree, int key, int *val)
{
    Tree_node **node = get_node_ptr(tree, key);

    if (*node == NULL)
        return false;

    if (val != NULL)
        *val = (*node)->val;
    tree_remove(node);

    return true;
}

bool search_tree_valid(Search_tree *tree)
{
    return valid_bin_search_tree(tree->root);
}

void search_tree_print(Search_tree *tree)
{
    tree_print(tree->root);
}
