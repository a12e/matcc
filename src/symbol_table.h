//
// Created by abrooke on 06/12/15.
//

#ifndef MATCC_SYMBOL_TABLE_H
#define MATCC_SYMBOL_TABLE_H

#include "hash_table.h"
#include "symbol.h"

extern hashtable_t symbol_table;

struct symbol *symbol_table_push(struct symbol *s);
struct symbol *symbol_table_lookup(char *name);
void symbol_table_print_variables();

#endif //MATCC_SYMBOL_TABLE_H
