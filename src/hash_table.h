// A dynamically-resized hash table using separate chaining.

typedef struct Hash_node {
    struct Hash_node *next;
    char *key;
    int val;
} Hash_node;

typedef struct Hash_table {
    Hash_node **buckets;
    size_t n_buckets;
    // The table is grown when the number of elements exceeds 'max_load'.
    size_t max_load;
    // Number of elements stored in the hash table.
    size_t n_elements;
} Hash_table;

void hash_table_init(Hash_table *hash_table);
void hash_table_free(Hash_table *hash_table);
// Sets 'key' to 'val'. If 'key' already exists, sets 'old_val' to its previous
// value and returns true. Otherwise, returns false and does not modify
// 'old_val'.
//
// The key is copied and so is safe to modify after calling this function.
bool hash_table_set(Hash_table *hash_table, const char *key, int val, int *old_val);
// If 'key' exists, sets 'val' to its value (if 'val' is not NULL) and returns
// true. Otherwise, returns false and does not modify 'val'.
bool hash_table_get(Hash_table *hash_table, const char *key, int *val);
// If 'key' exists, sets 'val' to its value and then removes it and returns
// true. Otherwise, returns false and does not modify 'val'.
bool hash_table_remove(Hash_table *hash_table, const char *key, int *val);
// Prints the representation of the hash table to stdout.
void hash_table_print(Hash_table *hash_table);
