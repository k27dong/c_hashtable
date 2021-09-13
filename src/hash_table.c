#include "hash_table.h"

ht_hash_table* ht_new_sized(int base_size) {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));

    ht->base_size = base_size;
    ht->size = next_prime(base_size);
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));

    return ht;
}

/**
 * Creates a new item
 * @param k The key chosen for the value
 * @param v The value of contained in this item
 */
ht_item* ht_new_item( char* k,  char* v) {
    ht_item* i = malloc(sizeof(ht_item));

    i->key = strdup(k);
    i->value = strdup(v);

    return i;
}

/**
 * Creates a new hashtable
 */
ht_hash_table* ht_new() {
    // ht_hash_table* ht = malloc(sizeof(ht_hash_table));

    // ht->size = 53;
    // ht->count = 0;
    // ht->items = calloc((size_t)ht->size, sizeof(ht_item*));

    // return ht;

    return ht_new_sized(INIT_SIZE);
}

/**
 * Deletes an item by completely remove it from the memory
 * @param i The item to be deleted
 */
void ht_del_item(ht_item* i) {
    free(i->key);
    free(i->value);
    free(i);
}

/**
 * Deletes a hash table
 * @param ht The hash table to be deleted
 */
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

/**
 * Generates a hash number by the value provided
 *
 * a is a prime number, m is the size of the hash table
 * For each letter in the value, the hash number, starting from zero, is incresed
 * by a^(index) * the ascii code for the letter. The result is a big number, it is
 * then modded by the size of the hash table, m, to get its position in the table
 *
 * @param s The value of the item
 * @param a A magic prime number used by this hash algorithm
 * @param m The size of the hash table, used for modular operation
 */
int hash( char* s,  int a,  int m) {
    long hash = 0;

    int len = strlen(s);

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


/**
 * Finds v by k
 * @param ht The hash table to look for
 * @param key The key to be searched for
 */
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
 * @param ht The hashtable which to be operated on
 * @param key
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

void ht_resize(ht_hash_table* ht, int base_size) {

}

/**
 * Checks whether the input number is prime
 * @param num The number to be checked on
 */
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

/**
 * Gets the next prime number after this number
 * @param num The number to be calculated on
 */
int next_prime(int num) {
    while (!is_prime(num)) {
        num++;
    }

    return num;
}