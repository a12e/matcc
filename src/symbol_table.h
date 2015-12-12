#ifndef MATCC_SYMBOL_TABLE_H
#define MATCC_SYMBOL_TABLE_H

#include <stdio.h>
#include "hash_table.h"
#include "symbol.h"

extern hashtable_t symbol_table;

struct symbol *symbol_table_push(struct symbol *s);
struct symbol *symbol_table_lookup(char *name);
void symbol_table_print_variables(FILE *f);
void symbol_table_print_data_section(FILE *f);

#define for_each_symbol(bucket) \
    for(size_t for_each_symbol_i = 0; for_each_symbol_i < symbol_table.size; for_each_symbol_i++) \
    if(symbol_table.buckets[for_each_symbol_i] != NULL) \
    for(ht_bucket *bucket = symbol_table.buckets[for_each_symbol_i]; bucket != NULL; bucket = bucket->successor)

#endif //MATCC_SYMBOL_TABLE_H
