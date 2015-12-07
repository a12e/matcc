#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "actions.h"
#include "symbol_table.h"
#include "quad_list.h"

extern int yylineno;

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
    exit(EXIT_FAILURE);
}

void ensure_type_match(struct symbol *s1, struct symbol *s2) {
    if(s1->type != s2->type) {
        abort_parsing("type of %s (%s) and %s (%s) do not match\n",
        s1->name, SYMBOL_TYPE_STR[s1->type], s2->name, SYMBOL_TYPE_STR[s2->type]);
    }
}

struct symbol *declare(enum symbol_type type, char *name) {
    return symbol_table_push(symbol_new(name, type));
}

struct symbol *assign(struct symbol *dest, struct symbol *src) {
    quad_gen(MOVE, dest, src, NULL);
    return dest;
}

struct symbol *declare_and_assign(enum symbol_type type, char *name, struct symbol *src) {
    struct symbol *s = declare(type, name);
    return assign(s, src);
}

struct symbol *declare_int_constant(int intval) {
    union symbol_initial_value value;
    value.intval = intval;
    return symbol_table_push(symbol_new_const(INT, value));
}

struct symbol *declare_float_constant(float floatval) {
    union symbol_initial_value value;
    value.floatval = floatval;
    return symbol_table_push(symbol_new_const(FLOAT, value));
}

struct symbol *add(struct symbol *op1, struct symbol *op2) {
    ensure_type_match(op1, op2);
    struct symbol *res = symbol_new_temp(op1->type);
    quad_gen(ADD, res, op1, op2);
    return symbol_table_push(res);
}

struct symbol *substract(struct symbol *op1, struct symbol *op2) {
    ensure_type_match(op1, op2);
    struct symbol *res = symbol_new_temp(op1->type);
    quad_gen(SUB, res, op1, op2);
    return symbol_table_push(res);
}

struct symbol *multiply(struct symbol *op1, struct symbol *op2) {
    ensure_type_match(op1, op2);
    struct symbol *res = symbol_new_temp(op1->type);
    quad_gen(MUL, res, op1, op2);
    return symbol_table_push(res);
}

struct symbol *divide(struct symbol *op1, struct symbol *op2) {
    ensure_type_match(op1, op2);
    struct symbol *res = symbol_new_temp(op1->type);
    quad_gen(DIV, res, op1, op2);
    return symbol_table_push(res);
}

