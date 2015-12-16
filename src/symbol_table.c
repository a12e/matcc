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

struct symbol *symbol_table_lookup_user(char *name) {
    char user_symbol[32];
    snprintf(user_symbol, 32, "_%s", name);
    return symbol_table_lookup(user_symbol);
}

void symbol_table_print_variables(FILE *f) {
    for(size_t i = 0; i < symbol_table.size; i++)
        if(symbol_table.buckets[i] != NULL) {
            for(ht_bucket *bucket = symbol_table.buckets[i]; bucket != NULL; bucket = bucket->successor)
                if(bucket->value->constant)
                    switch(bucket->value->type) {
                        case INT: fprintf(f, "%6s %6s %d\n", SYMBOL_TYPE_STR[bucket->value->type],
                                         bucket->value->name, bucket->value->initial_value.intval); break;
                        case FLOAT: fprintf(f, "%6s %6s %f\n", SYMBOL_TYPE_STR[bucket->value->type],
                                         bucket->value->name, bucket->value->initial_value.floatval); break;
                        case STRING: fprintf(f, "%6s %6s %s\n", SYMBOL_TYPE_STR[bucket->value->type],
                                            bucket->value->name, bucket->value->initial_value.stringval); break;
                        case MATRIX: fprintf(f, "%6s %6s (%zux%zu floats)\n", SYMBOL_TYPE_STR[bucket->value->type],
                                           bucket->value->name, bucket->value->initial_value.matrixval->height,
                                             bucket->value->initial_value.matrixval->width); break;
                    }
                else {
                    fprintf(f, "%6s %6s", SYMBOL_TYPE_STR[bucket->value->type], bucket->value->name);
                    if(bucket->value->type == MATRIX) fprintf(f, " (%zux%zu floats)",bucket->value->matrix_size.height,
                                                              bucket->value->matrix_size.width);
                    fprintf(f, "\n");
                }
        }
}

void symbol_table_print_data_section(FILE *f) {
    fprintf(f, ".data\n");
    for_each_symbol(bucket)
                if(bucket->value->constant) {
                    union symbol_initial_value initial_value = bucket->value->initial_value;
                    switch (bucket->value->type) {
                        case INT:
                            fprintf(f, "%s: .word %d\n", bucket->value->name, initial_value.intval);
                            break;
                        case FLOAT:
                            fprintf(f, "%s: .float %f\n", bucket->value->name, initial_value.floatval);
                            break;
                        case STRING:
                            fprintf(f, "%s: .align 4\n.asciiz %s\n", bucket->value->name, initial_value.stringval);
                            break;
                        case MATRIX:
                            fprintf(f, "%s: .word %zu\n.word %zu\n", bucket->value->name, initial_value.matrixval->height, initial_value.matrixval->width);
                            for(size_t l = 0; l < initial_value.matrixval->height; l++)
                                for(size_t c = 0; c < initial_value.matrixval->width; c++)
                                    fprintf(f, ".float %f\n", initial_value.matrixval->values[l * initial_value.matrixval->width + c]);
                            break;
                    }
                }
                else
                    switch(bucket->value->type) {
                        case INT: fprintf(f, "%s: .word 0\n", bucket->value->name); break;
                        case FLOAT: fprintf(f, "%s: .float 0.0\n", bucket->value->name); break;
                        case STRING: break;
                        case MATRIX: fprintf(f, "%s: .word %zu\n.word %zu\n.space %zu\n",
                                             bucket->value->name,
                                             bucket->value->matrix_size.height,
                                             bucket->value->matrix_size.width,
                                             4 * bucket->value->matrix_size.height
                                             * bucket->value->matrix_size.width); break;
                    }
}

