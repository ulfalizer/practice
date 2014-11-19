#include "common.h"
#include "queue.h"
#include "tree.h"
#include "vector.h"

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
        if (node_val == 0xDEAD) {
            // Makes e.g. tree_make(1, 0xDEAD) work too for creating an empty
            // tree, and means we crash reliably for malformed trees.
            nodes[i] = NULL;
            continue;
        }
        nodes[i] = emalloc(sizeof(Tree_node), "make tree, node");
        nodes[i]->key = node_val;
        nodes[i]->left = nodes[i]->right = NULL;
        // Is this the root node?
        if (i != 0) {
            // Nope, so point the correct child pointer in the parent to it.
            size_t parent = (i - 1)/2;
            if (i % 2 == 1)
                nodes[parent]->left = nodes[i];
            else
                nodes[parent]->right = nodes[i];
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

unsigned tree_depth(Tree_node *root) {
    if (root == NULL)
        return 0;
    return 1 + max(tree_depth(root->left), tree_depth(root->right));
}

void tree_rot_right(Tree_node **root) {
    Tree_node *left = (*root)->left;
    (*root)->left = left->right;
    left->right = *root;
    *root = left;
}

void tree_rot_left(Tree_node **root) {
    Tree_node *right = (*root)->right;
    (*root)->right = right->left;
    right->left = *root;
    *root = right;
}

bool tree_equals(Tree_node *root, size_t len, ...) {
    va_list ap;
    // Number of yet-to-be-expanded nodes. Zero means we've reached the end of
    // the tree.
    size_t n_nodes_left;
    Vector vector;

    // Do a breadth-first expansion with NULL for empty node positions. For
    // each node position, compare against the next argument.
    //
    // Simple, but wasteful if the tree is sparse.

    vector_init(&vector);
    vector_add(&vector, root);
    n_nodes_left = (root != NULL);
    va_start(ap, len);
    for (size_t i = 0; len > 0; ++i, --len) {
        Tree_node *node = vector_get(&vector, i);
        if (node == NULL) {
            if (va_arg(ap, int) != 0xDEAD)
                goto not_equal;
            // Expand the non-existing node to its two non-existing children.
            vector_add(&vector, NULL);
            vector_add(&vector, NULL);
        }
        else {
            if (node->key != va_arg(ap, int))
                goto not_equal;
            vector_add(&vector, node->left);
            vector_add(&vector, node->right);
            n_nodes_left =
              n_nodes_left - 1 + (node->left != NULL) + (node->right != NULL);
        }
    }
    va_end(ap);
    vector_free(&vector);
    return n_nodes_left == 0;

not_equal:
    va_end(ap);
    vector_free(&vector);
    return false;
}

// Bit silly not to implement tree_equals() in terms of this, but just for fun.
bool trees_equal(Tree_node *r1, Tree_node *r2) {
    if (r1 == NULL)
        return r2 == NULL;
    if (r2 == NULL)
        return r1 == NULL;
    return (r1->key == r2->key) &&
      trees_equal(r1->left, r2->left) &&
      trees_equal(r1->right, r2->right);
}

void tree_nodes_to_vector_dfs(Tree_node *node, Vector *vector) {
    if (node == NULL)
        return;
    tree_nodes_to_vector_dfs(node->left, vector);
    vector_add(vector, node);
    tree_nodes_to_vector_dfs(node->right, vector);
}

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

// Slightly convoluted to be able to handle INT_MIN and INT_MAX. The possible
// sets of valid numbers are
// [], [INT_MIN], [INT_MIN, INT_MIN+1], ..., [INT_MIN, ..., INT_MAX],
// which is one more than the number of representable values. NULL indicates
// "anything goes".
static bool valid_bin_search_tree_rec(Tree_node *root, int *max, int *min) {
    if (root == NULL)
        return true;
    if (max != NULL && root->key >= *max)
        return false;
    if (min != NULL && root->key <= *min)
        return false;
    return valid_bin_search_tree_rec(root->left, &root->key, min) &&
      valid_bin_search_tree_rec(root->right, max, &root->key);
}

bool valid_bin_search_tree(Tree_node *root) {
    if (root == NULL)
        return true;
    return valid_bin_search_tree_rec(root->left, &root->key, NULL) &&
      valid_bin_search_tree_rec(root->right, NULL, &root->key);
}

static void print_n_spaces(int n) {
    printf("%*s", n, "");
}

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
                printf("%*d", spacing, node->key);
                queue_add(&queue, node->left);
                queue_add(&queue, node->right);
            }
        }
        putchar('\n');
    }
    queue_free(&queue);
}
