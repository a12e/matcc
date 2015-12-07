//
// Created by abrooke on 10/11/15.
//

/*
 *  Table de hashage classique
 *  Copie les clés lors d'un ajout (ht_keycpy) mais pas les valeurs
 */

#ifndef MATCC_HASH_TABLE_H
#define MATCC_HASH_TABLE_H

#include <stdint.h>
#include <stddef.h>
#include "symbol.h"

typedef size_t ht_hash;
typedef char *ht_key;
typedef struct symbol *ht_val;

typedef struct ht_bucket_s {
    ht_key key;
    ht_val value;
    struct ht_bucket_s *successor;
} ht_bucket;

typedef enum {
    HT_OK = 0,
    HT_BAD_SIZE,
    HT_ALREADYEXISTS,
    HT_NOTFOUND,
} ht_error;

typedef struct {
    size_t size;
    ht_bucket **buckets;
    ht_error last_error;
} hashtable_t;

int         ht_keycmp   (ht_key key1, ht_key key2);
ht_key      ht_keycpy   (ht_key key);
ht_hash     ht_dohash   (hashtable_t *ht, ht_key key);
void        ht_init     (hashtable_t *ht, size_t size);
void        ht_put      (hashtable_t *ht, ht_key key, ht_val value);
ht_val      ht_get      (hashtable_t *ht, ht_key key);
int         ht_exists   (hashtable_t *ht, ht_key key);
void        ht_free     (hashtable_t *ht);
void        ht_dump     (hashtable_t *ht);
ht_error    ht_last_error(hashtable_t *ht);

#endif //MATCC_HASH_TABLE_H
