//
// Created by abrooke on 06/12/15.
//

#include "quad_list.h"
#include "utility.h"
#include "symbol.h"

struct quad_list *quad_list = NULL;
unsigned int printed_quad_lists = 0;

struct quad_list *quad_list_new(struct quad *quad) {
    struct quad_list *quad_list = safe_malloc(sizeof(struct quad_list));
    quad_list->node = quad;
    quad_list->successor = NULL;
    return quad_list;
}

void quad_list_push(struct quad_list **dest, struct quad *quad) {
    if(*dest == NULL) {
        *dest = quad_list_new(quad);
    }
    else {
        struct quad_list *l = *dest;
        for(; l->successor != NULL; l = l->successor);
        l->successor = quad_list_new(quad);
    }
}

void quad_gen(enum quad_op op, struct symbol *res, struct symbol *op1, struct symbol *op2) {
    quad_list_push(&quad_list, quad_new(op, res, op1, op2));
}

void quad_list_concat(struct quad_list **dest, struct quad_list *src) {
    if(*dest == NULL) {
        *dest = src;
    }
    else {
        struct quad_list *l = *dest;
        for(; l->successor != NULL; l = l->successor);
        l->successor = src;
    }
}

void quad_list_print(struct quad_list *list) {
    printf("L%02i:\n", printed_quad_lists++);
    for(; list != NULL; list = list->successor) {
        print_quad(list->node);
    }
}

