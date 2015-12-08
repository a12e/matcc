#include <stdarg.h>
#include <stddef.h>
#include "quad.h"
#include "utility.h"
#include "symbol.h"

unsigned int next_quad = 0;

struct quad *quad_new(enum quad_op op, struct symbol *res, struct symbol *op1, struct symbol *op2) {
    struct quad *q = safe_malloc(sizeof(struct quad));
    q->op = op;
    q->res = res;
    q->op1 = op1;
    q->op2 = op2;
    q->comment = NULL;
    next_quad++;
    return q;
}

struct quad *quad_new_empty(enum quad_op op) {
    return quad_new(op, NULL, NULL, NULL);
}

void quad_set_comment(struct quad *q, char *fmt, ...) {
    safe_free(q->comment);
    char comment[64];
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(comment, 64, fmt, argptr);
    va_end(argptr);
    q->comment = safe_strdup(comment);
}

const char *QUAD_OP_STR[MAX_QUAD] = {
        "nop",
        "move",
        "add",
        "sub",
        "mul",
        "div",
        "b",
        "be",
        "bne",
        "blt",
        "bgt",
        "blte",
        "bgte"
};

void quad_print(FILE *f, struct quad *q) {
    fprintf(f, "%4s", QUAD_OP_STR[q->op]);
    if(q->op2)
        fprintf(f, "%6s %6s %6s %s%s",
                q->res ? q->res->name : "?",
                q->op1 ? q->op1->name : "?",
                q->op2->name,
                q->comment ? " # " : "",
                q->comment ? q->comment : "");
    else if(q->op1)
        fprintf(f, "%6s %6s        %s%s",
                q->res ? q->res->name : "?",
                q->op1->name,
                q->comment ? " # " : "",
                q->comment ? q->comment : "");
    else
        fprintf(f, "%6s               %s%s",
                q->res ? q->res->name : "?",
                q->comment ? " # " : "",
                q->comment ? q->comment : "");
    fprintf(f, "\n");
}

void quad_delete(struct quad *q) {
    safe_free(q->comment);
    safe_free(q);
}



