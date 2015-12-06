//
// Created by abrooke on 17/11/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "actions.h"
#include "hash_table.h"
#include "instructions.h"
#include "parser.h"

extern int yylineno;
extern hashtable_t symbol_table;
extern iinstr_list instructions;

typedef uint bool;
#define true 1
#define false 0

bool allocated_registers[32] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0, // Temporary t0->t3 (not preserved across call)
        0, 0, 0, 0, // Temporary t4->t7 (not preserved across call)
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
};

const char *REGSTR[32] = {
        "", "", "", "",
        "", "", "", "",
        "$t0", "$t1", "$t2", "$t3",
        "$t4", "$t5", "$t6", "$t7",
        "", "", "", "",
        "", "", "", "",
        "", "", "", "",
        "", "", "", ""
};

void abort_parsing(char const *fmt, ...) {
    fprintf(stderr, "line %d: error: ", yylineno);

    va_list argptr;
    va_start(argptr, fmt);
    vfprintf(stderr, fmt, argptr);
    va_end(argptr);

    fprintf(stderr, "\n");
    exit(1);
}


register_id allocate_temp_register() {
    for(uint i = 8; i < 16; i++) {
        if(allocated_registers[i] == false) {
            allocated_registers[i] = true;
            return i;
        }
    }
    abort_parsing("Exhausted all temporary registers :(");
}

void free_register(register_id rid) {
    allocated_registers[rid] = false;
}

symbol *create_symbol(int type, register_id rid) {
    symbol *new_symbol = safe_malloc(sizeof(symbol));
    new_symbol->type = type;
    new_symbol->rid = rid;
    return new_symbol;
}

void free_symbol(symbol *s) {
    free(s);
}

register_id declare_int(char *name) {
    if(ht_exists(&symbol_table, name))
        abort_parsing("symbol '%s' has already been defined", name);

    register_id affected_register = allocate_temp_register();
    symbol *new_symbol = create_symbol(INT, affected_register);
    ht_put(&symbol_table, name, new_symbol);

    return affected_register;
}

register_id declare_int_with_init(char *name, register_id initial_value_register) {
    register_id new_symbol_register = declare_int(name);

    assign_to(new_symbol_register, initial_value_register);
    free_register(initial_value_register);

    return new_symbol_register;
}

register_id load_constant(int integer_constant) {
    register_id affected_register = allocate_temp_register();

    iinstr_put(&instructions, "li %s %d", REGSTR[affected_register], integer_constant);

    return affected_register;
}

register_id load_symbol(char *name) {
    symbol *loaded_symbol = ht_get(&symbol_table, name);

    if(ht_last_error(&symbol_table) == HT_NOTFOUND)
        abort_parsing("unknown symbol %s", name);

    return loaded_symbol->rid;
}

void assign_to(register_id destination, register_id source) {
    iinstr_put(&instructions, "move %s %s",
               REGSTR[destination],
               REGSTR[source]);
}

register_id int_add(register_id a, register_id b) {
    register_id result_register = allocate_temp_register();

    iinstr_put(&instructions, "add %s %s %s",
               REGSTR[result_register],
               REGSTR[a],
               REGSTR[b]);

    return result_register;
}
