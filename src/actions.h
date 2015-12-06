//
// Created by abrooke on 17/11/15.
//

#ifndef MATCC_ACTIONS_H
#define MATCC_ACTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "hash_table.h"
#include "instructions.h"
#include "utility.h"

typedef uint register_id;

typedef struct {
    int type;
    register_id rid;
} symbol;

register_id declare_int(char *name);
register_id declare_int_with_init(char *name, register_id initial_value_register);
register_id load_constant(int integer_constant);
register_id load_symbol(char *name);
void assign_to(register_id destination, register_id source);
register_id int_add(register_id a, register_id b);

#endif //MATCC_ACTIONS_H
