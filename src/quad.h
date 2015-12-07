//
// Created by abrooke on 06/12/15.
//

#ifndef MATCC_QUAD_H
#define MATCC_QUAD_H

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
void print_quad(struct quad *q);

#endif //MATCC_QUAD_H
