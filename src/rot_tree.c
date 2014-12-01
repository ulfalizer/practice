#include "common.h"
#include "rot_tree.h"
#include "tree.h"

// TODO: Could factor out some suitable traversal helpers to simplify.

void rot_tree_init(Rot_tree *tree) {
    tree->root = NULL;
}

void rot_tree_free(Rot_tree *tree) {
    tree_free(tree->root);
}

static void assemble(Rot_tree *tree,
  Tree_node *new_root,
  Tree_node **left, Tree_node **right,
  Tree_node **left_insert, Tree_node **right_insert) {
    tree->root = new_root;
    *left_insert = tree->root->left;
    *right_insert = tree->root->right;
    tree->root->left = *left;
    tree->root->right = *right;
}

// We perform the equivalent operations that a series of bottom-up rotations
// would do, but on the way down instead. The high-level intuition is that each
// right rotation on the way up will add some new stuff "above" the right
// subtree of the node being rotated up. To do things top-down, we instead add
// new stuff "below" in a temporary tree (at *right_insert). Left rotations
// work symmetrically, and we keep track of separate left and right trees that
// we build up (down) on the way down -- following a left link corresponds to a
// right rotation and vice versa. At the end, we glue things together
// appropriately using assemble().

#define DECLARE_LEFT_RIGHT                \
  Tree_node *left, *right;                \
  Tree_node **left_insert, **right_insert

#define INIT_LEFT_RIGHT \
  left = right = NULL;  \
  left_insert = &left;  \
  right_insert = &right

#define DECLARE_INIT_LEFT_RIGHT \
  DECLARE_LEFT_RIGHT;           \
  INIT_LEFT_RIGHT

// Inserts 'tree' below right tree.
#define INSERT_RIGHT(tree)     \
  *right_insert = tree;        \
  right_insert = &(tree)->left

// Inserts 'tree' below left tree.
#define INSERT_LEFT(tree)       \
  *left_insert = tree;          \
  left_insert = &(tree)->right

bool rot_tree_set(Rot_tree *tree, int key, int val, int *oldval) {
    DECLARE_INIT_LEFT_RIGHT;
    Tree_node **cur = &tree->root;

    while (*cur)
        if (key < (*cur)->key) {
            INSERT_RIGHT(*cur);
            cur = &(*cur)->left;
        }
        else if (key > (*cur)->key) {
            INSERT_LEFT(*cur);
            cur = &(*cur)->right;
        }
        else {
            if (oldval != NULL)
                *oldval = (*cur)->val;
            (*cur)->val = val;
            assemble(tree, *cur, &left, &right, left_insert, right_insert);
            return true;
        }

    *cur = create_node(key, val);
    assemble(tree, *cur, &left, &right, left_insert, right_insert);
    return false;
}

bool rot_tree_get(Rot_tree *tree, int key, int *val) {
    DECLARE_INIT_LEFT_RIGHT;
    Tree_node *cur = tree->root;

    if (tree->root == NULL)
        return false;

    for (;;)
        if (key < cur->key) {
            if (cur->left == NULL)
                break;
            INSERT_RIGHT(cur);
            cur = cur->left;
        }
        else if (key > cur->key) {
            if (cur->right == NULL)
                break;
            INSERT_LEFT(cur);
            cur = cur->right;
        }
        else {
            if (val != NULL)
                *val = cur->val;
            assemble(tree, cur, &left, &right, left_insert, right_insert);
            return true;
        }

    assemble(tree, cur, &left, &right, left_insert, right_insert);
    return false;
}

bool rot_tree_remove(Rot_tree *tree, int key, int *val) {
    DECLARE_LEFT_RIGHT;
    Tree_node *cur;
    Tree_node **rem;

    if (tree->root == NULL)
        return false;
    if (tree->root->key == key) {
        if (val != NULL)
            *val = tree->root->val;
        tree_remove(&tree->root);
        return true;
    }

    INIT_LEFT_RIGHT;

    cur = tree->root;
    for (;;)
        if (key < cur->key) {
            if (cur->left == NULL)
                break;
            if (cur->left->key == key) {
                rem = &cur->left;
                goto remove;
            }
            INSERT_RIGHT(cur);
            cur = cur->left;
        }
        else {
            if (cur->right == NULL)
                break;
            if (cur->right->key == key) {
                rem = &cur->right;
                goto remove;
            }
            INSERT_LEFT(cur);
            cur = cur->right;
        }

    assemble(tree, cur, &left, &right, left_insert, right_insert);
    return false;

remove:
    if (val != NULL)
        *val = (*rem)->val;
    tree_remove(rem);
    assemble(tree, cur, &left, &right, left_insert, right_insert);
    return true;
}

bool rot_tree_valid(Rot_tree *tree) {
    return valid_bin_search_tree(tree->root);
}

void rot_tree_print(Rot_tree *tree) {
    tree_print(tree->root);
}
