#include "common.h"
#include "hash_table.h"

#define INITIAL_BUCKETS 63
// The number of buckets is increased by a factor of GROWTH_FACTOR when the
// number of elements grows beyond MAX_LOAD*#buckets. There might be nicer ways
// to grow the table depending on the hash function.
#define MAX_LOAD 0.7
#define GROWTH_FACTOR 2

static unsigned long hash(const char *s) {
    // djb2 algorithm.
    unsigned long hash;

    for (hash = 5381; *s != '\0'; ++s)
        hash = ((hash << 5) + hash) + (uc)*s;
    return hash;
}

void hash_table_init(Hash_table *table) {
    table->buckets = emalloc(sizeof(Hash_node*)*INITIAL_BUCKETS, "hash init");
    for (size_t i = 0; i < INITIAL_BUCKETS; ++i)
        table->buckets[i] = NULL;
    table->n_buckets = INITIAL_BUCKETS;
    table->max_load = MAX_LOAD*INITIAL_BUCKETS;
    table->n_elements = 0;
}

void hash_table_free(Hash_table *table) {
    for (size_t i = 0; i < table->n_buckets; ++i) {
        Hash_node *next;
        for (Hash_node *node = table->buckets[i]; node; node = next) {
            next = node->next;
            free(node->key);
            free(node);
        }
    }
    free(table->buckets);
}

// Resizes the hash table (changes the number of buckets).
static void resize(Hash_table *table, size_t new_n_buckets) {
    Hash_node **new_buckets;

    // Allocate new set of initially empty buckets.
    new_buckets = emalloc(sizeof(Hash_node*)*new_n_buckets, "hash resize");
    for (size_t i = 0; i < new_n_buckets; ++i)
        new_buckets[i] = NULL;
    // Rehash keys.
    for (size_t i = 0; i < table->n_buckets; ++i) {
        Hash_node *next;
        for (Hash_node *node = table->buckets[i]; node; node = next) {
            unsigned long new_index;

            // Move node to new bucket.
            next = node->next;
            new_index = hash(node->key) % new_n_buckets;
            node->next = new_buckets[new_index];
            new_buckets[new_index] = node;
        }
    }

    free(table->buckets);
    table->buckets = new_buckets;
    // Calculate new maximum load from the new size.
    table->max_load = MAX_LOAD*new_n_buckets;
    table->n_buckets = new_n_buckets;
}

bool hash_table_set(Hash_table *table, const char *key, int val, int *old_val) {
    Hash_node *new_node, **bucket;

    bucket = table->buckets + hash(key) % table->n_buckets;

    // Is there already a node with the key?
    for (Hash_node *node = *bucket; node; node = node->next)
        if (strcmp(key, node->key) == 0) {
            // Yes, so just replace its value and return.
            if (old_val != NULL)
                *old_val = node->val;
            node->val = val;
            return true;
        }

    // We need to allocate a new node. Check if we exceed the max load for the
    // current number of buckets and grow the table if so.
    if (++table->n_elements > table->max_load) {
        resize(table, GROWTH_FACTOR*table->n_buckets);
        bucket = table->buckets + hash(key) % table->n_buckets;
    }

    new_node = emalloc(sizeof(Hash_node), "hash set, node");
    new_node->next = *bucket;
    new_node->key = estrdup(key, "hash set, key");
    new_node->val = val;
    *bucket = new_node;
    return false;
}

bool hash_table_get(Hash_table *table, const char *key, int *val) {
    for (Hash_node *node = table->buckets[hash(key) % table->n_buckets];
      node;
      node = node->next)
        if (strcmp(key, node->key) == 0) {
            if (val != NULL)
                *val = node->val;
            return true;
        }
    return false;
}

bool hash_table_remove(Hash_table *table, const char *key, int *val) {
    for (Hash_node **node = table->buckets + hash(key) % table->n_buckets;
      *node;
      node = &(*node)->next)
        if (strcmp(key, (*node)->key) == 0) {
            Hash_node *tmp;

            if (val != NULL)
                *val = (*node)->val;

            // Remove node.
            tmp = *node;
            *node = (*node)->next;
            free(tmp->key);
            free(tmp);

            --table->n_elements;
            return true;
        }
    return false;
}

void hash_table_print(Hash_table *table) {
    for (size_t i = 0; i < table->n_buckets; ++i) {
        printf("%zu:", i);
        for (Hash_node *node = table->buckets[i]; node; node = node->next)
            printf(" [ \"%s\" %d ]", node->key, node->val);
        putchar('\n');
    }
}
