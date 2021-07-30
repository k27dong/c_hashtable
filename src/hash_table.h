#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "common.h"

typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct {
    int size;
    int count;
    ht_item** items;
} ht_hash_table;

static ht_item HT_DELETED_ITEM = {NULL, NULL};

ht_item* ht_new_item(char* k, char* v);
ht_hash_table* ht_new();
static void ht_del_item(ht_item*);
void ht_del_hash_table(ht_hash_table*);
int hash(char* s, int a, int m);
int ht_get_hash(char* s, int ht_len, int attempt);

void ht_insert(ht_hash_table* ht, char* key, char* value);
char* ht_search(ht_hash_table* ht, char* key);
void ht_delete(ht_hash_table* ht, char* key);

int is_prime(int);
int next_prime(int);