// A dynamically-resized hash table using separate chaining

#include "common.h"
#include "hash_table.h"

#define INITIAL_BUCKETS 63
// The table size is increased by a factor of GROWTH_FACTOR when the number of
// elements grows beyond MAX_LOAD*#buckets. There might be nicer ways to grow
// the table depending on the hash function.
#define MAX_LOAD 0.7
#define GROWTH_FACTOR 2

static unsigned long hash(const char *s) {
    // djb2 algorithm
    unsigned long hash = 5381;
    while (*s)
        hash = ((hash << 5) + hash) + (unsigned char)*s++;
    return hash;
}

void hash_table_init(Hash_table *hash_table) {
    hash_table->buckets =
      emalloc(sizeof(Hash_node*)*INITIAL_BUCKETS, "hash init");
    for (size_t i = 0; i < INITIAL_BUCKETS; ++i)
        hash_table->buckets[i] = NULL;
    hash_table->n_buckets = INITIAL_BUCKETS;
    hash_table->max_load = MAX_LOAD*INITIAL_BUCKETS;
    hash_table->n_elements = 0;
}

void hash_table_free(Hash_table *hash_table) {
    for (size_t i = 0; i < hash_table->n_buckets; ++i) {
        Hash_node *next;
        for (Hash_node *node = hash_table->buckets[i]; node; node = next) {
            next = node->next;
            free(node->key);
            free(node);
        }
    }
    free(hash_table->buckets);
}

static void resize(Hash_table *hash_table, size_t new_size) {
    Hash_node **new_buckets;

    new_buckets = emalloc(sizeof(Hash_node*)*new_size, "hash resize");
    for (size_t i = 0; i < new_size; ++i)
        new_buckets[i] = NULL;
    for (size_t i = 0; i < hash_table->n_buckets; ++i) {
        Hash_node *next;
        for (Hash_node *node = hash_table->buckets[i]; node; node = next) {
            unsigned long new_index;

            next = node->next;
            new_index = hash(node->key) % new_size;
            node->next = new_buckets[new_index];
            new_buckets[new_index] = node;
        }
    }

    free(hash_table->buckets);
    hash_table->buckets = new_buckets;
    hash_table->max_load = MAX_LOAD*new_size;
    hash_table->n_buckets = new_size;
}

bool hash_table_set(Hash_table *hash_table, const char *key, int val, int *old_val) {
    Hash_node *new_node, **bucket;

    bucket = hash_table->buckets + hash(key) % hash_table->n_buckets;

    // Is there already a node with the key?
    for (Hash_node **node = bucket; *node; node = &(*node)->next) {
        if (strcmp(key, (*node)->key) == 0) {
            if (old_val != NULL)
                *old_val = (*node)->val;
            (*node)->val = val;
            return true;
        }
    }

    if (++hash_table->n_elements > hash_table->max_load) {
        resize(hash_table, GROWTH_FACTOR*hash_table->n_buckets);
        bucket = hash_table->buckets + hash(key) % hash_table->n_buckets;
    }

    new_node = emalloc(sizeof(Hash_node), "hash set, node");
    new_node->next = *bucket;
    new_node->key = estrdup(key, "hash set, key");
    new_node->val = val;
    *bucket = new_node;

    return false;
}

bool hash_table_get(Hash_table *hash_table, const char *key, int *val) {
    for (Hash_node *node = hash_table->buckets[hash(key) % hash_table->n_buckets];
      node;
      node = node->next)
        if (strcmp(key, node->key) == 0) {
            if (val !=  NULL)
                *val = node->val;
            return true;
        }
    return false;
}

bool hash_table_remove(Hash_table *hash_table, const char *key, int *val) {
    for (Hash_node **node = hash_table->buckets + hash(key) % hash_table->n_buckets;
      *node;
      node = &(*node)->next)
        if (strcmp(key, (*node)->key) == 0) {
            if (val != NULL)
                *val = (*node)->val;
            Hash_node *tmp = *node;
            *node = (*node)->next;
            free(tmp->key);
            free(tmp);
            --hash_table->n_elements;
            return true;
        }
    return false;
}

void hash_table_print(Hash_table *hash_table) {
    for (size_t i = 0; i < hash_table->n_buckets; ++i) {
        printf("%zu:", i);
        for (Hash_node *node = hash_table->buckets[i]; node; node = node->next)
            printf(" [ \"%s\" %d ]", node->key, node->val);
        putchar('\n');
    }
}
