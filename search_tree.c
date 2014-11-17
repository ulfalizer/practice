// Non-self-balancing binary search tree

#include "common.h"
#include "search_tree.h"
#include "tree.h"

void search_tree_init(Search_tree *tree) {
    tree->root = NULL;
}

void search_tree_free(Search_tree *tree) {
    tree_free(tree->root);
}

void search_tree_print(Search_tree *tree) {
    tree_print(tree->root);
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
    *cur = emalloc(sizeof(Tree_node), "search tree add");
    (*cur)->left = (*cur)->right = NULL;
    (*cur)->key = key;
    (*cur)->val = val;
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

static Tree_node *unlink_max(Tree_node **cur) {
    Tree_node *res;
    while ((*cur)->right != NULL)
        cur = &(*cur)->right;
    res = *cur;
    *cur = (*cur)->left;
    return res;
}

// Avoid collision with remove() from stdio
static void remove_(Tree_node **node) {
    if ((*node)->left == NULL) {
        Tree_node *tmp = *node;
        *node = (*node)->right;
        free(tmp);
    }
    else if ((*node)->right == NULL) {
        Tree_node *tmp = *node;
        *node = (*node)->left;
        free(tmp);
    }
    else {
        Tree_node *max = unlink_max(&(*node)->left);
        (*node)->key = max->key;
        (*node)->val = max->val;
        free(max);
    }
}

bool search_tree_remove(Search_tree *tree, int key, int *oldval) {
    Tree_node **cur = &tree->root;
    while (*cur)
        if (key < (*cur)->key)
            cur = &(*cur)->left;
        else if (key > (*cur)->key)
            cur = &(*cur)->right;
        else {
            if (oldval != NULL)
                *oldval = (*cur)->val;
            remove_(cur);
            return true;
        }
    return false;
}

// Test helper
bool search_tree_valid(Search_tree *tree) {
    return valid_bin_search_tree(tree->root);
}
