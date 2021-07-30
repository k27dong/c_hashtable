#include "hash_table.h"

ht_item* ht_new_item( char* k,  char* v) {
    ht_item* i = malloc(sizeof(ht_item));

    i->key = strdup(k);
    i->value = strdup(v);

    return i;
}

ht_hash_table* ht_new() {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));

    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));

    return ht;
}

static void ht_del_item(ht_item* i) {
    free(i->key);
    free(i->value);
    free(i);
}

void ht_del_hash_table(ht_hash_table* ht) {
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];

        if (item != NULL) {
            ht_del_item(item);
        }
    }

    free(ht->items);
    free(ht);
}

int hash( char* s,  int a,  int m) {
    long hash = 0;

    const int len = strlen(s);

    for (int i = 0; i < len; i++) {
        hash += (long) pow(a, len - i - 1) * s[i];
        // hash = hash % m;
        hash %= m;
    }

    return (int) hash;
}

int ht_get_hash(char* s,  int ht_size,  int attempt) {
    const int hash_a = hash(s, PRIME_1, ht_size);
    const int hash_b = hash(s, PRIME_2, ht_size);

    return (hash_a + (attempt * (hash_b + 1))) % ht_size;
}

void ht_insert(ht_hash_table* ht, char* key, char* value) {
    int i = 1;
    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);

    ht_item* curr_item = ht->items[index];
    while (curr_item != NULL) {
        if (curr_item != &HT_DELETED_ITEM && strcmp(curr_item->key, key) == 0) {
            ht_del_item(curr_item);
            ht->items[index] = item;
            return;
        }

        index = ht_get_hash(item->key, ht->size, i++);
        curr_item = ht->items[index];
    }

    ht->items[index] = item;
    ht->count++;
}


/* find v by k */
char* ht_search(ht_hash_table* ht, char* key) {
    int i = 1;
    int index = ht_get_hash(key, ht->size, 0);

    ht_item* item = ht->items[index];
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM && strcmp(item->key, key) == 0) {
            return item->value;
        }

        index = ht_get_hash(key, ht->size, i++);
        item = ht->items[index];
    }

    return NULL;    /* should never get here */
}

/**
 * deleting an item in the collision chain will break the chain
 * instead replace it with a marker
 * shifting every item in the chain will resulting in O(n)
 * which is not ideal
 */
void ht_delete(ht_hash_table* ht, char* key) {
    int i = 1;
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];

    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }

        index = ht_get_hash(key, ht->size, i++);
        item = ht->items[index];
    }

    ht->count--;
}

int is_prime(int num) {
    if (num <= 1) {
        return 0;
    }

    if (num % 2 == 0 && num > 2) {
        return 0;
    }

    for (int i = 3; i <= floor(sqrt((double) num)); i += 2) {
        if (!(num % i)) {
            return 0;
        }
    }

    return 1;
}

int next_prime(int num) {
    while (!is_prime(num)) {
        num++;
    }

    return num;
}