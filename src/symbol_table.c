#include "symbol_table.h"
#include "parsing/actions.h"

hashtable_t symbol_table;

struct symbol *symbol_table_push(struct symbol *s) {
    ht_put(&symbol_table, s->name, s);

    if(ht_last_error(&symbol_table) == HT_ALREADYEXISTS)
        abort_parsing("symbol %s is already defined", s->name);

    return s;
}

struct symbol *symbol_table_lookup(char *name) {
    struct symbol *s = ht_get(&symbol_table, name);

    if(ht_last_error(&symbol_table) == HT_NOTFOUND)
        abort_parsing("unknown symbol %s", name);

    return s;
}

void symbol_table_print_variables(FILE *f) {
    for(size_t i = 0; i < symbol_table.size; i++)
        if(symbol_table.buckets[i] != NULL) {
            for(ht_bucket *bucket = symbol_table.buckets[i]; bucket != NULL; bucket = bucket->successor)
                if(bucket->value->constant)
                    switch(bucket->value->type) {
                        case INT: fprintf(f, "%5s %6s %6d\n", SYMBOL_TYPE_STR[bucket->value->type],
                                         bucket->value->name, bucket->value->initial_value.intval); break;
                        case FLOAT: fprintf(f, "%5s %6s %6f\n", SYMBOL_TYPE_STR[bucket->value->type],
                                         bucket->value->name, bucket->value->initial_value.floatval); break;
                        case MATRIX: fprintf(f, "%5s %6s %6f\n", SYMBOL_TYPE_STR[bucket->value->type], // TODO
                                           bucket->value->name, bucket->value->initial_value.floatval); break;
                    }
                else
                    fprintf(f, "%5s %6s\n", SYMBOL_TYPE_STR[bucket->value->type], bucket->value->name);
        }
}

void symbol_table_print_data_section(FILE *f) {
    fprintf(f, ".data\n");
    for_each_symbol(bucket) {
                switch(bucket->value->type) {
                    case INT: fprintf(f, "%s: .word %d\n", bucket->value->name, bucket->value->initial_value.intval); break;
                    case FLOAT: fprintf(f, "%s: .float 0.0\n", bucket->value->name); break;
                    case MATRIX: fprintf(f, "%s: .space 42\n", bucket->value->name); break;
                }
            }
}

