typedef struct Vector Vector;

typedef struct Tree_node {
    struct Tree_node *left, *right;
    int key;
    int val;
} Tree_node;

Tree_node *tree_make(size_t len, ...);
void tree_free(Tree_node *root);
void tree_nodes_to_vector_dfs(Tree_node *root, Vector *vector);
void tree_nodes_to_vector_bfs(Tree_node *root, Vector *vector);
unsigned tree_depth(Tree_node *root);
bool valid_bin_search_tree(Tree_node *root);
void tree_print(Tree_node *root);
