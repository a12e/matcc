#ifndef MATCC_SYMBOL_ARRAY_H
#define MATCC_SYMBOL_ARRAY_H

#include <stddef.h>
#include <symbol.h>

struct symbol_array {
    struct symbol **symbols;
    size_t size;
};

void symbol_array_push(struct symbol_array *array, struct symbol *s);
void symbol_array_remove(struct symbol_array *array, size_t index);
void symbol_array_free(struct symbol_array *array);
int symbols_sort_inc_start_fn(const void *s1, const void *s2);
int symbols_sort_inc_end_fn(const void *s1, const void *s2);

#endif //MATCC_SYMBOL_ARRAY_H
