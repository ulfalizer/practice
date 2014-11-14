// TODO: Make dynamic
#define HASH_N_BUCKETS 5

typedef struct Hash_node {
    struct Hash_node *next;
    const char *key;
    int val;
} Hash_node;

typedef struct Hash_table {
    Hash_node *buckets[HASH_N_BUCKETS];
} Hash_table;

void hash_table_init(Hash_table *hash_table);
void hash_table_free(Hash_table *hash_table);
bool hash_table_set(Hash_table *hash_table, const char *key, int val, int *old_val);
bool hash_table_get(Hash_table *hash_table, const char *key, int *val);
bool hash_table_remove(Hash_table *hash_table, const char *key, int *val);
void hash_table_print(Hash_table *hash_table);
