#include "common.h"
#include "queue.h"
#include "stack.h"
#include "tree.h"
#include "vector.h"

Tree_node *create_node(int key, int val, Tree_node *left, Tree_node *right) {
    Tree_node *node = emalloc(sizeof(Tree_node), "create tree node");
    node->key = key;
    node->val = val;
    node->left = left;
    node->right = right;
    return node;
}

Tree_node *tree_make(size_t len, ...) {
    va_list ap;
    Tree_node **nodes;

    if (len == 0)
        return NULL;

    // Build an array of node pointers (like in a heap) and use it to construct
    // the tree.
    nodes = alloca(sizeof(Tree_node*)*len);

    va_start(ap, len);
    for (size_t i = 0; i < len; ++i) {
        int node_key = va_arg(ap, int);
        if (node_key == 0xDEAD) {
            // Makes e.g. tree_make(1, 0xDEAD) work too for creating an empty
            // tree, and means we crash reliably for malformed trees.
            nodes[i] = NULL;

            continue;
        }
        nodes[i] = create_node(node_key, node_key + 1, NULL, NULL);
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

    return nodes[0];
}

void tree_free(Tree_node *root) {
    if (root == NULL)
        return;
    tree_free(root->left);
    tree_free(root->right);
    free(root);
}

Tree_node *tree_copy(Tree_node *root) {
    if (root == NULL)
        return NULL;
    return create_node(root->key, root->val,
      tree_copy(root->left), tree_copy(root->right));
}

unsigned tree_depth(Tree_node *root) {
    if (root == NULL)
        return 0;
    return 1 + max(tree_depth(root->left), tree_depth(root->right));
}

void tree_rot_right(Tree_node **node) {
    Tree_node *left = (*node)->left;
    (*node)->left = left->right;
    left->right = *node;
    *node = left;
}

void tree_rot_left(Tree_node **node) {
    Tree_node *right = (*node)->right;
    (*node)->right = right->left;
    right->left = *node;
    *node = right;
}

static Tree_node *unlink_max(Tree_node **cur) {
    Tree_node *res;
    while ((*cur)->right != NULL)
        cur = &(*cur)->right;
    res = *cur;
    *cur = (*cur)->left;
    return res;
}

void tree_remove(Tree_node **node) {
    Tree_node *rem;
    if ((*node)->left == NULL) {
        rem = *node;
        *node = (*node)->right;
    }
    else if ((*node)->right == NULL) {
        rem = *node;
        *node = (*node)->left;
    }
    else {
        rem = unlink_max(&(*node)->left);
        (*node)->key = rem->key;
        (*node)->val = rem->val;
    }
    free(rem);
}

bool tree_equals(Tree_node *root, size_t len, ...) {
    va_list ap;
    // Number of yet-to-be-expanded nodes. Zero means we've reached the end of
    // the tree.
    size_t n_nodes_left;
    Queue queue;

    // Do a breadth-first expansion with NULL for empty node positions. For
    // each node position, compare against the next argument.
    //
    // Simple, but wasteful if the tree is sparse.

    queue_init(&queue);
    queue_add(&queue, root);
    n_nodes_left = (root != NULL);
    va_start(ap, len);
    for (size_t i = 0; i < len; ++i) {
        int key_arg = va_arg(ap, int);
        Tree_node *node = queue_remove(&queue);
        if (node == NULL) {
            if (key_arg != 0xDEAD)
                goto not_equal;
            // Expand the non-existing node to its two non-existing children.
            queue_add(&queue, NULL);
            queue_add(&queue, NULL);
        }
        else {
            if (node->key != key_arg)
                goto not_equal;
            queue_add(&queue, node->left);
            queue_add(&queue, node->right);
            n_nodes_left =
              n_nodes_left - 1 + (node->left != NULL) + (node->right != NULL);
        }
    }
    va_end(ap);
    queue_free(&queue);
    return n_nodes_left == 0;

not_equal:
    va_end(ap);
    queue_free(&queue);
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

void tree_nodes_to_vector_iter_preorder(Tree_node *node, Vector *vector) {
    Stack stack;

    stack_init(&stack);
    for (;;) {
        for (; node != NULL; node = node->left) {
            vector_add(vector, node);
            stack_push(&stack, node);
        }

        if (stack_len(&stack) == 0)
            break;

        node = ((Tree_node*)stack_pop(&stack))->right;
    }
    stack_free(&stack);
}

void tree_nodes_to_vector_iter_inorder(Tree_node *node, Vector *vector) {
    Stack stack;

    stack_init(&stack);
    for (;;) {
        for (; node != NULL; node = node->left)
            stack_push(&stack, node);

        if (stack_len(&stack) == 0)
            break;

        node = stack_pop(&stack);
        vector_add(vector, node);
        node = node->right;
    }
    stack_free(&stack);
}

void tree_nodes_to_vector_iter_postorder(Tree_node *node, Vector *vector) {
    Stack stack;

    stack_init(&stack);
    for (;;) {
        Tree_node *parent;

        for (; node != NULL; node = node->left)
            stack_push(&stack, node);

        // We know the parent has no left child at this point, which makes the
        // code below work out for all cases.

move_up:
        if (stack_len(&stack) == 0)
            break;

        // 'parent' is the parent of 'node'.
        parent = stack_peek(&stack);
        if (node == parent->right) {
            // We arrived from the right (or skipped an empty right tree). Pop
            // the parent, add it to the result, and move up the tree.
            stack_pop(&stack);
            vector_add(vector, parent);
            node = parent;

            goto move_up;
        }
        // We arrived from the left. Keep the parent on the stack and process
        // its right subtree.
        node = parent->right;
    }
    stack_free(&stack);
}

bool tree_dfs_iter(Tree_node *node, int key, int *val) {
    Stack stack;

    stack_init(&stack);
    for (;;) {
        for (; node != NULL; node = node->left) {
            if (node->key == key) {
                if (val != NULL)
                    *val = node->val;

                stack_free(&stack);
                return true;
            }
            stack_push(&stack, node);
        }

        if (stack_len(&stack) == 0) {
            stack_free(&stack);
            return false;
        }

        node = ((Tree_node*)stack_pop(&stack))->right;
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
    if ((max != NULL && root->key >= *max) ||
        (min != NULL && root->key <= *min))
        return false;
    return valid_bin_search_tree_rec(root->left, &root->key, min) &&
      valid_bin_search_tree_rec(root->right, max, &root->key);
}

bool valid_bin_search_tree(Tree_node *root) {
    return valid_bin_search_tree_rec(root, NULL, NULL);
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
