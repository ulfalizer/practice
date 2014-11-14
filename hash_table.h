typedef struct Hash_table Hash_table;

Hash_table *hash_table_make();
void hash_table_free(Hash_table *hash_table);
bool hash_table_set(Hash_table *hash_table, const char *key, int val, int *old_val);
bool hash_table_get(Hash_table *hash_table, const char *key, int *val);
bool hash_table_remove(Hash_table *hash_table, const char *key, int *val);
void hash_table_print(Hash_table *hash_table);
