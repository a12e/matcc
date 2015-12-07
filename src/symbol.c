//
// Created by abrooke on 06/12/15.
//

#include "symbol.h"
#include "utility.h"

unsigned int temps_count = 0;
unsigned int constants_count = 0;

const char *SYMBOL_TYPE_STR[] = {
        "int", "float", "matrix"
};

struct symbol *symbol_new(char *name, enum symbol_type type) {
    struct symbol *s = safe_malloc(sizeof(struct symbol));
    s->name = safe_strdup(name);
    s->type = type;
    s->temporary = false;
    s->constant = false;
    return s;
}

struct symbol *symbol_new_temp(enum symbol_type type) {
    char temp_name[32];
    snprintf(temp_name, 32, "temp_%d", temps_count++);
    struct symbol *s = symbol_new(temp_name, type);
    s->temporary = true;
    return s;
}

struct symbol *symbol_new_const(enum symbol_type type, union symbol_initial_value value) {
    char constant_name[32];
    snprintf(constant_name, 32, "const_%d", constants_count++);
    struct symbol *s = symbol_new(constant_name, type);
    s->initial_value = value;
    s->constant = true;
    return s;
}

void symbol_delete(struct symbol *s) {
    safe_free(s->name);
    safe_free(s);
}
