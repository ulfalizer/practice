// Binary tree stuff.

typedef struct Vector Vector;

typedef struct Tree_node {
    struct Tree_node *left, *right;
    int key;
    int val;
} Tree_node;

// Builds a tree out of the variable argument list (with 'len' entries). 0xDEAD
// specifies the lack of a node. The last level does not need to be fully
// specified.
//
// The key of each node is set to the specified value, while the value is set
// to the key plus one -- having them be different is handy for testing.
//
// Usage example assuming _ == 0xDEAD:
//
// tree_make(15,
//          2,
//      1,      5,
//    8,  9,  _,  9,
//   _,4,_,6,_,_,3,2)
Tree_node *tree_make(size_t len, ...);
void tree_free(Tree_node *root);

// Returns a new tree with copies of all nodes.
Tree_node *tree_copy(Tree_node *root);

// Creates a new tree node with no children.
Tree_node *create_node(int key, int val);

// Returns the number of levels in the tree -- zero for an empty tree, one
// for a tree with one node, etc.
unsigned tree_depth(Tree_node *root);

// Does a right rotation at 'node', bringing up its left child.
void tree_rot_right(Tree_node **node);
// Does a left rotation at 'node', bringing up its right child.
void tree_rot_left(Tree_node **node);

// Performs a binary search tree removal of 'node'. Replaces with predecessor
// if the node has a left child.
void tree_remove(Tree_node **node);

// Return true if the tree matches the variable argument list. The format is
// the same as for tree_make(). Supports extra trailing 0xDEAD entries in the
// variable argument list, which is intuitive.
bool tree_equals(Tree_node *root, size_t len, ...);

// Returns true if the trees rooted at 'r1' and 'r2' are equal.
bool trees_equal(Tree_node *r1, Tree_node *r2);

// Adds all tree nodes in-order into the vector.
void tree_nodes_to_vector_dfs(Tree_node *root, Vector *vector);

// Adds all tree nodes level-order into the vector.
void tree_nodes_to_vector_bfs(Tree_node *root, Vector *vector);

// Adds all tree nodes pre-order into the vector (non-recursive).
void tree_nodes_to_vector_iter_preorder(Tree_node *node, Vector *vector);

// Adds all tree nodes in-order into the vector (non-recursive).
void tree_nodes_to_vector_iter_inorder(Tree_node *node, Vector *vector);

// Adds all tree nodes post-order into the vector (non-recursive).
void tree_nodes_to_vector_iter_postorder(Tree_node *node, Vector *vector);

// If 'key' exists in the tree, sets 'val' to its value (if 'val' is not NULL)
// and returns true. Otherwise, returns false and does not modify 'val'.
// Implemented non-recursively.
bool tree_dfs_iter(Tree_node *node, int key, int *val);

// Returns true if the tree is a valid binary search tree
// (<left subtree nodes> < <node> < <right subtree nodes>, for all nodes).
bool valid_bin_search_tree(Tree_node *root);

// Prints a tree to stdout. The format is similar to the input format in the
// tree_make() example (but without commas).
void tree_print(Tree_node *root);
