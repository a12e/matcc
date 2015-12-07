//
// Created by abrooke on 06/12/15.
//

#include <stddef.h>
#include "quad.h"
#include "utility.h"
#include "symbol.h"

struct quad *quad_new(enum quad_op op, struct symbol *res, struct symbol *op1, struct symbol *op2) {
    struct quad *q = safe_malloc(sizeof(struct quad));
    q->op = op;
    q->res = res;
    q->op1 = op1;
    q->op2 = op2;
    return q;
}

const char *QUAD_OP_STR[MAX_QUAD] = {
        "move",
        "add",
        "sub",
        "mul",
        "div"
};

void print_quad(struct quad *q) {
    if(q->op2)
        printf("%4s %8s %8s %8s\n", QUAD_OP_STR[q->op], q->res->name, q->op1->name, q->op2->name);
    else
        printf("%4s %8s %8s\n", QUAD_OP_STR[q->op], q->res->name, q->op1->name);
}


