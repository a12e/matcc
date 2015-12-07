#ifndef MATCC_SYMBOL_TABLE_H
#define MATCC_SYMBOL_TABLE_H

#include <stdio.h>
#include "hash_table.h"
#include "symbol.h"

extern hashtable_t symbol_table;

struct symbol *symbol_table_push(struct symbol *s);
struct symbol *symbol_table_lookup(char *name);
void symbol_table_print_variables(FILE *f);

#endif //MATCC_SYMBOL_TABLE_H
