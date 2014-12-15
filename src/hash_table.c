#include "common.h"
#include "hash_table.h"

// Keep the number of buckets as a power of two to keep things simple and speed
// up the modulos operation. The downside is that we're effectively truncating
// the result of the hash function to the lowest n bits, which could decrease
// entropy.
#define INITIAL_BUCKETS 64
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

// Returns a pointer to the start of the bucket for 'key' in 'table'.
static Hash_node **get_bucket(Hash_table *table, const char *key) {
    return table->buckets + (hash(key) & (table->n_buckets - 1));
}

// Creates and initializes a new set of 'n_buckets' empty buckets in 'table'.
// Does not update 'n_elements'.
static void init_buckets(Hash_table *table, size_t n_buckets) {
    table->buckets = emalloc(sizeof(Hash_node*)*n_buckets, "hash buckets init");
    for (size_t i = 0; i < n_buckets; ++i)
        table->buckets[i] = NULL;
    table->n_buckets = n_buckets;
    // Calculate maximum load from the bucket count.
    table->max_load = MAX_LOAD*n_buckets;
}

void hash_table_init(Hash_table *table) {
    init_buckets(table, INITIAL_BUCKETS);
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
    Hash_node **old_buckets = table->buckets;
    size_t old_n_buckets = table->n_buckets;

    // Allocate new set of initially empty buckets.
    init_buckets(table, new_n_buckets);

    // Re-insert the old elements, re-hashing their keys.
    for (size_t i = 0; i < old_n_buckets; ++i) {
        Hash_node *next;
        for (Hash_node *node = old_buckets[i]; node; node = next) {
            Hash_node **bucket;

            next = node->next;

            // Move node to new bucket.
            bucket = get_bucket(table, node->key);
            node->next = *bucket;
            *bucket = node;
        }
    }
    free(old_buckets);
}

bool hash_table_set(Hash_table *table, const char *key, int val, int *old_val) {
    Hash_node *new_node, **bucket;

    bucket = get_bucket(table, key);

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
        bucket = get_bucket(table, key);
    }

    new_node = emalloc(sizeof(Hash_node), "hash set, node");
    new_node->next = *bucket;
    new_node->key = estrdup(key, "hash set, key");
    new_node->val = val;
    *bucket = new_node;
    return false;
}

bool hash_table_get(Hash_table *table, const char *key, int *val) {
    for (Hash_node *node = *get_bucket(table, key); node; node = node->next)
        if (strcmp(key, node->key) == 0) {
            if (val != NULL)
                *val = node->val;
            return true;
        }
    return false;
}

bool hash_table_remove(Hash_table *table, const char *key, int *val) {
    for (Hash_node **node = get_bucket(table, key); *node;
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
