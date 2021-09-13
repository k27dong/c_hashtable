#ifndef HTABLE_H
#define HTABLE_H

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "common.h"

/**
 * two structs are needed: ht_hash_table which presents the table and
 * ht_item for each item.
 *
 * ht_item: each item in the table contains a key and a value
 * ht_hash_table:
 *  base_size   : the base size of this hash table
 *  size        : the maximum number of items this table can store
 *  count       : the number of items this table is storing
 *  items       : points to an array of ht_item
 *
 * HT_DELETED_ITEM: a placeholder used to represent a deleted item
 */

typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct {
    int base_size;
    int size;
    int count;
    ht_item** items;
} ht_hash_table;

static ht_item HT_DELETED_ITEM = {NULL, NULL};

ht_item* ht_new_item(char* k, char* v);
ht_hash_table* ht_new();
ht_hash_table* ht_new_sized(int base_size);
void ht_del_item(ht_item*);
void ht_del_hash_table(ht_hash_table*);
int hash(char* s, int a, int m);
int ht_get_hash(char* s, int ht_len, int attempt);

void ht_insert(ht_hash_table* ht, char* key, char* value);
char* ht_search(ht_hash_table* ht, char* key);
void ht_delete(ht_hash_table* ht, char* key);

int is_prime(int);
int next_prime(int);

#endif