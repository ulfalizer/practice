// Binary tree stuff

#include "common.h"
#include "queue.h"
#include "tree.h"
#include "vector.h"

// Builds a tree out of the variable argument list (with 'len' entries).
// INT_MIN specifies the lack of a node. The last level does not need to be
// fully specified.
//
// Usage example assuming _ == INT_MIN:
//
// tree_make(15,
//          2,
//      1,      5,
//    8,  9,  _,  9,
//   _,4,_,6,_,_,3,2)
Tree_node *tree_make(size_t len, ...) {
    va_list ap;
    Tree_node **nodes, *res;

    if (len == 0)
        return NULL;

    // Build an array of node pointers (like in a heap) and use it to construct
    // the tree.
    nodes = emalloc(sizeof(Tree_node*)*len, "make tree, pointer array");
    va_start(ap, len);
    for (size_t i = 0; i < len; ++i) {
        int node_val = va_arg(ap, int);
        if (node_val == INT_MIN)
            nodes[i] = NULL;
        else {
            nodes[i] = emalloc(sizeof(Tree_node), "make tree, node");
            nodes[i]->val = node_val;
            nodes[i]->left = nodes[i]->right = NULL;
            if (i != 0) {
                size_t parent = (i - 1)/2;
                if (nodes[parent] == NULL)
                    fail("%s: some nodes lack parents", __func__);
                if (i % 2 == 1)
                    nodes[parent]->left = nodes[i];
                else
                    nodes[parent]->right = nodes[i];
            }
        }
    }
    va_end(ap);
    res = nodes[0];
    free(nodes);
    return res;
}

void tree_free(Tree_node *root) {
    if (root == NULL)
        return;
    tree_free(root->left);
    tree_free(root->right);
    free(root);
}

// Adds all tree nodes in-order into the vector.
void tree_nodes_to_vector_dfs(Tree_node *node, Vector *vector) {
    if (node == NULL)
        return;
    tree_nodes_to_vector_dfs(node->left, vector);
    vector_add(vector, node);
    tree_nodes_to_vector_dfs(node->right, vector);
}

// Adds all tree nodes level-order into the vector.
void tree_nodes_to_vector_bfs(Tree_node *root, Vector *vector) {
    if (root != NULL)
        vector_add(vector, root);
    for (size_t i = vector_len(vector) - 1; i < vector_len(vector); ++i) {
        Tree_node *node = vector_get(vector, i);
        if (node->left != NULL)
            vector_add(vector, node->left);
        if (node->right != NULL)
            vector_add(vector, node->right);
    }
}

unsigned tree_depth(Tree_node *root) {
    if (root == NULL)
        return 0;
    return 1 + max(tree_depth(root->left), tree_depth(root->right));
}

static void print_n_spaces(int n) {
    printf("%*s", n, "");
}

// Debugging helper that prints a tree. The format is similar to the input
// format in the tree_make() example (but without commas).
void tree_print(Tree_node *root) {
    unsigned depth = tree_depth(root);
    Queue queue;

    queue_init(&queue);
    queue_add(&queue, root);
    // Step through the tree level-by-level. For each of the 2^n node positions
    // at level n, print either the value at that position or just spaces if no
    // node exists.
    for (unsigned level = 0; level < depth; ++level) {
        for (size_t i = 0; i < (1 << level); ++i) {
            Tree_node *node = queue_remove(&queue);
            // Use the depth of the tree to calculate the needed spacing. The
            // spacing before the first element of the level is half the
            // spacing before the remaining elements.
            unsigned spacing = 1 << (depth - level - (i == 0));
            if (node == NULL) {
                print_n_spaces(spacing);
                queue_add(&queue, NULL);
                queue_add(&queue, NULL);
            }
            else {
                printf("%*d", spacing, node->val);
                queue_add(&queue, node->left);
                queue_add(&queue, node->right);
            }
        }
        putchar('\n');
    }
    queue_free(&queue);
}
