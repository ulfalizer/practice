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
// Usage example assuming _ == 0xDEAD:
//
// tree_make(15,
//          2,
//      1,      5,
//    8,  9,  _,  9,
//   _,4,_,6,_,_,3,2)
Tree_node *tree_make(size_t len, ...);
void tree_free(Tree_node *root);
// Returns the number of levels in the tree -- zero for an empty tree, one
// for a tree with one node, etc.
unsigned tree_depth(Tree_node *root);
void tree_rot_right(Tree_node **root);
void tree_rot_left(Tree_node **root);
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
// Returns true if the tree is a valid binary search tree
// (<left subtree nodes> < <node> < <right subtree nodes>, for all nodes).
bool valid_bin_search_tree(Tree_node *root);
// Prints a tree to stdout. The format is similar to the input format in the
// tree_make() example (but without commas).
void tree_print(Tree_node *root);
