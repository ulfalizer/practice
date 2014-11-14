#include "common.h"
#include "hash_table.h"

static size_t hash(const char *s) {
    // djb2 algorithm
    unsigned long hash = 5381;
    while (*s)
        hash = ((hash << 5) + hash) + (unsigned char)*s++;
    return hash % HASH_N_BUCKETS;
}

void hash_table_init(Hash_table *hash_table) {
    for (size_t i = 0; i < HASH_N_BUCKETS; ++i)
        hash_table->buckets[i] = NULL;
}

void hash_table_free(Hash_table *hash_table) {
    for (size_t i = 0; i < HASH_N_BUCKETS; ++i) {
        Hash_node *next;
        for (Hash_node *node = hash_table->buckets[i]; node; node = next) {
            next = node->next;
            free(node);
        }
    }
}

bool hash_table_set(Hash_table *hash_table, const char *key, int val, int *old_val) {
    Hash_node *new, **node;
    for (node = hash_table->buckets + hash(key); *node; node = &(*node)->next) {
        if (strcmp(key, (*node)->key) == 0) {
            if (old_val != NULL)
                *old_val = (*node)->val;
            (*node)->val = val;
            return true;
        }
    }
    new = malloc(sizeof(Hash_node));
    if (new == NULL)
        err("malloc Hash_node");
    new->next = *node;
    new->key = key;
    new->val = val;
    *node = new;
    return false;
}

bool hash_table_get(Hash_table *hash_table, const char *key, int *val) {
    for (Hash_node *node = hash_table->buckets[hash(key)];
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
    for (Hash_node **node = hash_table->buckets + hash(key);
      *node;
      node = &(*node)->next)
        if (strcmp(key, (*node)->key) == 0) {
            if (val != NULL)
                *val = (*node)->val;
            Hash_node *tmp = *node;
            *node = (*node)->next;
            free(tmp);
            return true;
        }
    return false;
}

void hash_table_print(Hash_table *hash_table) {
    for (size_t i = 0; i < HASH_N_BUCKETS; ++i) {
        printf("%zu:", i);
        for (Hash_node *node = hash_table->buckets[i]; node; node = node->next)
            printf(" [ \"%s\" %d ]", node->key, node->val);
        putchar('\n');
    }
}
