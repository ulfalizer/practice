// Non-self-balancing binary search tree.

typedef struct Tree_node Tree_node;

typedef struct Search_tree {
    Tree_node *root;
} Search_tree;

void search_tree_init(Search_tree *tree);
void search_tree_free(Search_tree *tree);

// Sets 'key' to 'val'. If 'key' already exists, sets 'old_val' to its previous
// value (if 'old_val' is not NULL) and returns true. Otherwise, returns false
// without modifying 'old_val'.
bool search_tree_set(Search_tree *tree, int key, int val, int *old_val);

// If 'key' exists, sets 'val' to its value (if 'val' is not NULL) and returns
// true. Otherwise, returns false without modifying 'val'.
bool search_tree_get(Search_tree *tree, int key, int *val);

// If 'key' exists, sets 'val' to its value (if 'val' is not NULL) and then
// removes it and returns true. Otherwise, returns false without modifying
// 'val'.
bool search_tree_remove(Search_tree *tree, int key, int *val);

// Prints the representation of the search tree to stdout.
void search_tree_print(Search_tree *tree);

// Test helper. Returns true if the internal representation is a valid binary
// search tree.
bool search_tree_valid(Search_tree *tree);
