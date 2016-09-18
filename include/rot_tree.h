// Top-down implementation of rotate-to-root tree - simpler than splay tree but
// no worst-case amortized O(log(n)) bound

typedef struct Tree_node Tree_node;

typedef struct Rot_tree {
    Tree_node *root;
} Rot_tree;

void rot_tree_init(Rot_tree *tree);
void rot_tree_free(Rot_tree *tree);

// Sets 'key' to 'val'. If 'key' already exists, sets 'old_val' to its previous
// value (if 'old_val' is not NULL) and returns true. Otherwise, returns false
// without modifying 'old_val'.
bool rot_tree_set(Rot_tree *tree, int key, int val, int *old_val);

// If 'key' exists, sets 'val' to its value (if 'val' is not NULL) and returns
// true. Otherwise, returns false without modifying 'val'.
bool rot_tree_get(Rot_tree *tree, int key, int *val);

// If 'key' exists, sets 'val' to its value (if 'val' is not NULL) and then
// removes it and returns true. Otherwise, returns false without modifying
// 'val'.
bool rot_tree_remove(Rot_tree *tree, int key, int *val);

// Prints the representation of the rotate-to-root tree to stdout
void rot_tree_print(Rot_tree *tree);

// Test helper - returns true if the internal representation is a valid binary
// search tree
bool rot_tree_valid(Rot_tree *tree);
