#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "data_types.h"

/* Problem-specific */
#define HASH_TABLE_BASED_SIZE      503   /* prime, large enough for word search */
#define HASH_TABLE_INCREASING_RATE  70   /* resize when 70% full               */
#define MAX_KEY_SIZE                 8   /* "XXXXXX\0" = 7 bytes, 8 is safe    */

typedef struct {
    unsigned int   size;
    unsigned int   count;
    unsigned char **State_Key;
} Hash_Table;

/* Problem-specific */
void Generate_HashTable_Key(const State *const state, unsigned char *key);

/* Framework */
Hash_Table* New_Hash_Table(const int size);
void        Resize_Hash_Table(Hash_Table *ht, const int size);
void        Delete_Hash_Table(Hash_Table *ht);
void        ht_insert(Hash_Table *ht, const State *const state);
void        ht_insert_key(Hash_Table *ht, const char *key);
int         ht_search(Hash_Table *ht, const State *const state);
void        Show_Hash_Table(Hash_Table *ht);

#endif
