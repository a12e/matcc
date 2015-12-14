#include <utility.h>
#include <string.h>
#include "symbol_array.h"

void symbol_array_push(struct symbol_array *array, struct symbol *s) {
    array->size++;
    array->symbols = safe_realloc(array->symbols, array->size * sizeof(struct symbol *));
    array->symbols[array->size - 1] = s;
}

void symbol_array_remove(struct symbol_array *array, size_t index) {
    if(index != array->size - 1)
        memmove(&array->symbols[index], &array->symbols[index + 1],
                (array->size - index - 1) * sizeof(struct symbol *));
    array->size--;
    array->symbols = safe_realloc(array->symbols, array->size * sizeof(struct symbol *));
}

void symbol_array_free(struct symbol_array *array) {
    safe_free(array->symbols);
    array->size = 0;
}

int symbols_sort_inc_start_fn(const void *s1, const void *s2) {
    return (*(const struct symbol **) s1)->start_point - (*(const struct symbol **) s2)->start_point;
}

int symbols_sort_inc_end_fn(const void *s1, const void *s2) {
    return (*(const struct symbol **) s1)->end_point - (*(const struct symbol **) s2)->end_point;
}