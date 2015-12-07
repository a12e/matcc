//
// Created by abrooke on 06/12/15.
//

#ifndef MATCC_QUAD_LIST_H
#define MATCC_QUAD_LIST_H

#include "quad.h"

struct quad_list {
    struct quad *node;
    struct quad_list *successor;
};

extern struct quad_list *quad_list;

struct quad_list *quad_list_new(struct quad *quad);
void quad_list_push(struct quad_list **dest, struct quad *quad);
void quad_gen(enum quad_op op, struct symbol *res, struct symbol *op1, struct symbol *op2);
void quad_list_concat(struct quad_list **dest, struct quad_list *src);
void quad_list_print(struct quad_list *list);

#endif //MATCC_QUAD_LIST_H
