#ifndef MATCC_ACTIONS_H
#define MATCC_ACTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "hash_table.h"
#include "utility.h"
#include "symbol.h"
#include "quad.h"
#include "quad_list.h"

void abort_parsing(char const *fmt, ...);
void ensure_type_match(struct symbol *s1, struct symbol *s2);

struct expr_attr {
    struct quad_list *code;
    struct symbol *symbol;
};

struct symbol *declare(enum symbol_type type, char *name);
struct expr_attr assign(struct symbol *dest, struct expr_attr src);
struct expr_attr declare_and_assign(enum symbol_type type, char *name, struct expr_attr expr);
struct expr_attr declare_int_constant(int intval);
struct expr_attr declare_float_constant(float floatval);

struct expr_attr binary_arithmetic_op(struct expr_attr expr1, enum quad_op op, struct expr_attr expr2);

struct cond_attr {
    struct quad_list *code;
    struct quad_list *true_list;
    struct quad_list *false_list;
};

struct cond_attr condition_or(struct cond_attr cond1, struct symbol *tag, struct cond_attr cond2);
struct cond_attr condition_and(struct cond_attr cond1, struct symbol *tag, struct cond_attr cond2);
struct cond_attr condition_not(struct cond_attr op);
struct cond_attr condition_true();
struct cond_attr condition_false();
struct cond_attr condition_compare_expressions(struct expr_attr expr1, enum quad_op quad_op, struct expr_attr expr2);

struct quad_list *control_if(struct cond_attr condition, struct symbol *true_tag, struct quad_list *true_statements,
                               struct symbol *end_tag);
struct quad_list *control_if_else(struct cond_attr condition, struct symbol *true_tag,
                                  struct quad_list *true_statements, struct symbol *false_tag,
                                  struct quad_list *false_statements, struct symbol *end_tag);
struct quad_list *control_while(struct symbol *condition_tag, struct cond_attr condition, struct symbol *statements_tag,
                                struct quad_list *statements, struct symbol *end_tag);
struct quad_list *control_do_while(struct symbol *statements_tag, struct quad_list *statements,
                                   struct cond_attr condition, struct symbol *end_tag);
struct quad_list *control_for(struct expr_attr variable_declaration, struct symbol *condition_tag,
                              struct cond_attr condition, struct symbol *iteration_tag, struct expr_attr iteration,
                              struct symbol *statements_tag, struct quad_list *statements, struct symbol *end_tag);

#endif //MATCC_ACTIONS_H
