// A dynamically-resized hash table using separate chaining

typedef struct Hash_node {
    struct Hash_node *next;
    char *key;
    int val;
} Hash_node;

typedef struct Hash_table {
    Hash_node **buckets;
    size_t n_buckets;
    // Number of elements stored in the hash table
    size_t n_elements;
    // The table is grown when the number of elements exceeds 'max_load'
    size_t max_load;
} Hash_table;

void hash_table_init(Hash_table *table);
void hash_table_free(Hash_table *table);

// Sets 'key' to 'val'. If 'key' already exists, sets 'old_val' to its previous
// value (if 'old_val' is not NULL) and returns true. Otherwise, returns false
// without modifying 'old_val'.
//
// The key is copied and so is safe to modify after calling this function.
bool hash_table_set(Hash_table *table, const char *key, int val, int *old_val);

// If 'key' exists, sets 'val' to its value (if 'val' is not NULL) and returns
// true. Otherwise, returns false without modifying 'val'.
bool hash_table_get(Hash_table *table, const char *key, int *val);

// If 'key' exists, sets 'val' to its value (if 'val' is not NULL) and then
// removes it and returns true. Otherwise, returns false without modifying
// 'val'.
bool hash_table_remove(Hash_table *table, const char *key, int *val);

// Prints the representation of the hash table to stdout
void hash_table_print(Hash_table *table);
