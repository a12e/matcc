#include <stdlib.h>
#include <string.h>
#include "hash_table.h"
#include "utility.h"

int ht_keycmp(ht_key key1, ht_key key2) {
    return strcmp(key1, key2);
}

ht_key ht_keycpy(ht_key key) {
    return safe_strdup(key);
}

ht_hash ht_dohash(hashtable_t *ht, ht_key key) {
    ht_hash sum = 0;

    for(int i = 0; key[i] != '\0'; i++)
        sum += key[i];

    return sum % ht->size;
}

void ht_init(hashtable_t *ht, size_t size) {
    if(size < 1) {
        ht->last_error = HT_BAD_SIZE;
        return;
    }

    ht->size = size;
    ht->buckets = safe_calloc(size, sizeof(ht_bucket *));
    ht->last_error = HT_OK;
}

void ht_put(hashtable_t *ht, ht_key key, ht_val value) {
    ht_hash hash = ht_dohash(ht, key);

    if(ht_exists(ht, key)) {
        ht->last_error = HT_ALREADYEXISTS;
        return;
    }

    ht_bucket *item = safe_malloc(sizeof(ht_bucket));
    item->key = ht_keycpy(key);
    item->value = value;
    item->successor = ht->buckets[hash];
    ht->buckets[hash] = item;
    ht->last_error = HT_OK;
}

ht_val ht_get(hashtable_t *ht, ht_key key) {
    ht_hash hash = ht_dohash(ht, key);

    for(ht_bucket *item = ht->buckets[hash]; item != NULL; item = item->successor)
        if(ht_keycmp(item->key, key) == 0) {
            ht->last_error = HT_OK;
            return item->value;
        }

    ht->last_error = HT_NOTFOUND;
    return NULL;
}

int ht_exists(hashtable_t *ht, ht_key key) {
    ht_get(ht, key);
    if(ht->last_error != HT_NOTFOUND) {
        ht->last_error = HT_OK;
        return 1;
    }
    return 0;
}

void ht_free(hashtable_t *ht) {
    for(size_t i = 0; i < ht->size; i++)
        for(ht_bucket *item = ht->buckets[i]; item != NULL;) {
            safe_free(item->key);
            symbol_delete(item->value);
            ht_bucket *successor = item->successor;
            safe_free(item);
            item = successor;
        }
    safe_free(ht->buckets);
}

void ht_dump(hashtable_t *ht) {
    printf("hashtable (%lu buckets)\n", ht->size);
    for(size_t i = 0; i < ht->size; i++)
        if(ht->buckets[i] != NULL) {
            printf("  bucket #%lu (%p)\n", i, ht->buckets[i]);
            for(ht_bucket *item = ht->buckets[i]; item != NULL; item = item->successor)
                printf("    - %s = %p\n", item->key, item->value);
        }
}

ht_error ht_last_error(hashtable_t *ht) {
    ht_error error = ht->last_error;
    ht->last_error = HT_OK;
    return error;
}
