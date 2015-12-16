#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "actions.h"
#include "symbol_table.h"

extern int yylineno;

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
    return symbol_table_push(symbol_new_user(name, type));
}

struct expr_attr assign(struct symbol *dest, struct expr_attr src) {
    struct expr_attr result;

    struct quad *q = quad_new(MOVE, dest, src.symbol, NULL);
    quad_set_comment(q, "%s = %d", dest->name, src.symbol->initial_value.intval); // TODO (temp)
    result.code = quad_list_concat(2, src.code, quad_list_new(q));
    result.symbol = dest;

    return result;
}

struct expr_attr declare_and_assign(enum symbol_type type, char *name, struct expr_attr expr) {
    struct symbol *s = declare(type, name);
    return assign(s, expr);
}

struct expr_attr declare_int_constant(int intval) {
    struct expr_attr result;
    union symbol_initial_value value;

    value.intval = intval;
    result.symbol = symbol_table_push(symbol_new_const(INT, value));
    result.code = NULL;

    return result;
}

struct expr_attr declare_float_constant(float floatval) {
    struct expr_attr result;
    union symbol_initial_value value;

    value.floatval = floatval;
    result.symbol = symbol_table_push(symbol_new_const(FLOAT, value));
    result.code = NULL;

    return result;
}

struct expr_attr declare_string_constant(char *stringval) {
    struct expr_attr result;
    union symbol_initial_value value;

    value.stringval = stringval;
    result.symbol = symbol_table_push(symbol_new_const(STRING, value));
    result.code = NULL;

    return result;
}

struct expr_attr declare_matrix_constant(struct matrix *matrix) {
    struct expr_attr result;
    union symbol_initial_value value;

    value.matrixval = matrix;
    result.symbol = symbol_table_push(symbol_new_const(MATRIX, value));
    result.code = NULL;

    return result;
}

struct matrix_size declare_matrix_size(size_t height, size_t width) {
    struct matrix_size result;
    result.height = height;
    result.width = width;
    return result;
}

struct symbol *declare_matrix(char *name, struct matrix_size size) {
    struct symbol *s = declare(MATRIX, name);
    s->size = 8 + size.height * size.width * 4;
    s->matrix_size.height = size.height;
    s->matrix_size.width = size.width;
    return s;
}

struct expr_attr unary_arithmetic_op(enum quad_op op, struct expr_attr expr) {
    struct expr_attr result;
    result.symbol = symbol_new_temp(expr.symbol->type);
    result.code = quad_list_new(quad_new(op, result.symbol, expr.symbol, NULL));
    symbol_table_push(result.symbol);

    return result;
}

struct expr_attr binary_arithmetic_op(struct expr_attr expr1, enum quad_op op, struct expr_attr expr2) {
    ensure_type_match(expr1.symbol, expr2.symbol);

    struct expr_attr result;
    result.symbol = symbol_new_temp(expr1.symbol->type);
    result.code = quad_list_new(quad_new(op, result.symbol, expr1.symbol, expr2.symbol));
    symbol_table_push(result.symbol);

    return result;
}

struct cond_attr condition_or(struct cond_attr cond1, struct symbol *tag, struct cond_attr cond2) {
    quad_list_complete(cond1.false_list, tag); // si la première condition est fausse, les sauts doivent aller sur la 2è

    struct cond_attr result;
    result.code = quad_list_concat(2, cond1.code, cond2.code);
    result.true_list = quad_list_concat(2, cond1.true_list, cond2.true_list);
    result.false_list = cond2.false_list;

    return result;
}

struct cond_attr condition_and(struct cond_attr cond1, struct symbol *tag, struct cond_attr cond2) {
    quad_list_complete(cond1.false_list, tag);

    struct cond_attr result;
    result.code = quad_list_concat(2, cond1.code, cond2.code);
    result.false_list = quad_list_concat(2, cond1.false_list, cond2.false_list);
    result.true_list = cond2.true_list;

    return result;
}

struct cond_attr condition_not(struct cond_attr op) {
    struct cond_attr result;
    result.code = op.code;
    result.true_list = op.false_list;
    result.false_list = op.true_list;

    return result;
}

struct cond_attr condition_true() {
    struct cond_attr result;
    result.code = quad_list_new(quad_new_empty(B));
    result.true_list = result.code;
    result.false_list = NULL;

    return result;
}

struct cond_attr condition_false() {
    struct cond_attr result;
    result.code = quad_list_new(quad_new_empty(B));
    result.true_list = NULL;
    result.false_list = result.code;

    return result;
}

struct cond_attr condition_compare_expressions(struct expr_attr expr1, enum quad_op quad_op, struct expr_attr expr2) {
    struct cond_attr result;

    struct quad *goto_true = quad_new(quad_op, NULL, expr1.symbol, expr2.symbol);
    quad_set_comment(goto_true, "jump to the true statements");
    struct quad_list *goto_true_list = quad_list_new(goto_true);

    struct quad *goto_false = quad_new_empty(B);
    quad_set_comment(goto_false, "jump to the false statements");
    struct quad_list *goto_false_list = quad_list_new(goto_false);

    result.code = quad_list_concat(4, expr1.code, expr2.code, goto_true_list, goto_false_list);
    result.true_list = goto_true_list;
    result.false_list = goto_false_list;

    return result;
}

struct quad_list *control_if(struct cond_attr condition, struct symbol *true_tag, struct quad_list *true_statements,
                               struct symbol *end_tag) {
    quad_list_complete(condition.true_list, true_tag);
    quad_list_complete(condition.false_list, end_tag);

    return quad_list_concat(2, condition.code, true_statements);
}

struct quad_list *control_if_else(struct cond_attr condition, struct symbol *true_tag,
                                  struct quad_list *true_statements, struct symbol *false_tag,
                                  struct quad_list *false_statements, struct symbol *end_tag) {
    quad_list_complete(condition.true_list, true_tag);
    quad_list_complete(condition.false_list, false_tag);

    struct quad *goto_end = quad_new(B, end_tag, NULL, NULL);
    quad_set_comment(goto_end, "jump to the end of if-else control");

    return quad_list_concat(4, condition.code, true_statements, quad_list_new(goto_end), false_statements);
}

struct quad_list *control_while(struct symbol *condition_tag, struct cond_attr condition, struct symbol *statements_tag,
                                struct quad_list *statements, struct symbol *end_tag) {
    quad_list_complete(condition.true_list, statements_tag);
    quad_list_complete(condition.false_list, end_tag);

    struct quad *goto_condition = quad_new(B, condition_tag, NULL, NULL);
    quad_set_comment(goto_condition, "jump to the condition of while control");

    return quad_list_concat(3, condition.code, statements, quad_list_new(goto_condition));
}

struct quad_list *control_do_while(struct symbol *statements_tag, struct quad_list *statements,
                                   struct cond_attr condition, struct symbol *end_tag) {
    quad_list_complete(condition.true_list, statements_tag);
    quad_list_complete(condition.false_list, end_tag);

    return quad_list_concat(2, statements, condition.code);
}

struct quad_list *control_for(struct expr_attr variable_declaration, struct symbol *condition_tag,
                              struct cond_attr condition, struct symbol *iteration_tag, struct expr_attr iteration,
                              struct symbol *statements_tag, struct quad_list *statements, struct symbol *end_tag) {
    quad_list_complete(condition.true_list, statements_tag);
    quad_list_complete(condition.false_list, end_tag);

    struct quad *goto_condition = quad_new(B, condition_tag, NULL, NULL);
    quad_set_comment(goto_condition, "jump to the condition of for control");
    struct quad *goto_iteration = quad_new(B, iteration_tag, NULL, NULL);
    quad_set_comment(goto_iteration, "jump to the iteration of for control");

    return quad_list_concat(6, variable_declaration.code, condition.code, iteration.code,
                            quad_list_new(goto_condition), statements, quad_list_new(goto_iteration));
}

struct quad_list *control_return() {
    return quad_list_new(quad_new_empty(RET));
}

struct quad_list *call_function_with_expr(char *function_name, struct expr_attr expr) {
    if(strcmp(function_name, "print") == 0) {
        switch(expr.symbol->type) {
            case INT: return quad_list_concat(2, expr.code, quad_list_new(quad_new(PRTI, expr.symbol, NULL, NULL)));
            case FLOAT: return quad_list_concat(2, expr.code, quad_list_new(quad_new(PRTF, expr.symbol, NULL, NULL)));
            case STRING: return quad_list_concat(2, expr.code, quad_list_new(quad_new(PRTS, expr.symbol, NULL, NULL)));
            case MATRIX: return NULL; // TODO
        }
    }
    else {
        abort_parsing("unknown function name %s", function_name);
        return NULL;
    }
}
