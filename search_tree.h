typedef struct Tree_node Tree_node;

typedef struct Search_tree {
    Tree_node *root;
} Search_tree;

void search_tree_init(Search_tree *tree);
void search_tree_free(Search_tree *tree);
void search_tree_print(Search_tree *tree);
bool search_tree_set(Search_tree *tree, int key, int val, int *oldval);
bool search_tree_get(Search_tree *tree, int key, int *val);
bool search_tree_remove(Search_tree *tree, int key, int *oldval);

bool search_tree_valid(Search_tree *tree);
