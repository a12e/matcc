#include "symbol_table.h"
#include "actions.h"

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
                        case INT: fprintf(f, ".%s %8s %8d\n", SYMBOL_TYPE_STR[bucket->value->type],
                                         bucket->value->name, bucket->value->initial_value.intval); break;
                        case FLOAT: fprintf(f, ".%s %8s %8f\n", SYMBOL_TYPE_STR[bucket->value->type],
                                         bucket->value->name, bucket->value->initial_value.floatval); break;
                        case MATRIX: fprintf(f, ".%s %8s %8f\n", SYMBOL_TYPE_STR[bucket->value->type], // TODO
                                           bucket->value->name, bucket->value->initial_value.floatval); break;
                    }
                else
                    fprintf(f, ".%s %8s\n", SYMBOL_TYPE_STR[bucket->value->type], bucket->value->name);
        }
}
