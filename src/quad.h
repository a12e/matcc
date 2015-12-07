#ifndef MATCC_QUAD_H
#define MATCC_QUAD_H

#include <stdio.h>

enum quad_op {
    MOVE,
    ADD,
    SUB,
    MUL,
    DIV,
    MAX_QUAD
};

struct quad {
    enum quad_op op;
    struct symbol *res;
    struct symbol *op1;
    struct symbol *op2;
};

struct quad *quad_new(enum quad_op op, struct symbol *res, struct symbol *op1, struct symbol *op2);
void print_quad(FILE *f, struct quad *q);
void quad_delete(struct quad *q);

#endif //MATCC_QUAD_H
