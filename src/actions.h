#ifndef MATCC_ACTIONS_H
#define MATCC_ACTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "hash_table.h"
#include "utility.h"
#include "symbol.h"

void abort_parsing(char const *fmt, ...);
void ensure_type_match(struct symbol *s1, struct symbol *s2);

struct symbol *declare(enum symbol_type type, char *name);
struct symbol *assign(struct symbol *dest, struct symbol *src);
struct symbol *declare_and_assign(enum symbol_type type, char *name, struct symbol *src);
struct symbol *declare_int_constant(int intval);
struct symbol *declare_float_constant(float floatval);

struct symbol *add(struct symbol *op1, struct symbol *op2);
struct symbol *substract(struct symbol *op1, struct symbol *op2);
struct symbol *multiply(struct symbol *op1, struct symbol *op2);
struct symbol *divide(struct symbol *op1, struct symbol *op2);

#endif //MATCC_ACTIONS_H
