#ifndef MATCC_QUAD_H
#define MATCC_QUAD_H

#include <stdio.h>

enum quad_op {
    NOP,
    MOVE,
    ADD,
    SUB,
    MUL,
    DIV,
    B,
    BE,
    BNE,
    BLT,
    BGT,
    BLTE,
    BGTE,
    MAX_QUAD
};

struct quad {
    enum quad_op op;
    struct symbol *res;
    struct symbol *op1;
    struct symbol *op2;
    char *comment;
};

extern unsigned int next_quad;

struct quad *quad_new(enum quad_op op, struct symbol *res, struct symbol *op1, struct symbol *op2);
struct quad *quad_new_empty(enum quad_op op);
void quad_set_comment(struct quad *q, char *fmt, ...);
void quad_print(FILE *f, struct quad *q);
void quad_delete(struct quad *q);

#endif //MATCC_QUAD_H
