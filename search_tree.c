#include "common.h"
#include "search_tree.h"
#include "tree.h"

void search_tree_init(Search_tree *tree) {
    tree->root = NULL;
}

void search_tree_free(Search_tree *tree) {
    tree_free(tree->root);
}

bool search_tree_set(Search_tree *tree, int key, int val, int *oldval) {
    Tree_node **cur = &tree->root;
    while (*cur)
        if (key < (*cur)->key)
            cur = &(*cur)->left;
        else if (key > (*cur)->key)
            cur = &(*cur)->right;
        else {
            if (oldval != NULL)
                *oldval = (*cur)->val;
            (*cur)->val = val;
            return true;
        }
    *cur = create_node(key, val);
    return false;
}

bool search_tree_get(Search_tree *tree, int key, int *val) {
    Tree_node *cur = tree->root;
    while (cur)
        if (key < cur->key)
            cur = cur->left;
        else if (key > cur->key)
            cur = cur->right;
        else {
            if (val != NULL)
                *val = cur->val;
            return true;
        }
    return false;
}

bool search_tree_remove(Search_tree *tree, int key, int *val) {
    Tree_node **cur = &tree->root;
    while (*cur)
        if (key < (*cur)->key)
            cur = &(*cur)->left;
        else if (key > (*cur)->key)
            cur = &(*cur)->right;
        else {
            if (val != NULL)
                *val = (*cur)->val;
            tree_remove(cur);
            return true;
        }
    return false;
}

bool search_tree_valid(Search_tree *tree) {
    return valid_bin_search_tree(tree->root);
}

void search_tree_print(Search_tree *tree) {
    tree_print(tree->root);
}
