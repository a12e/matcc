#ifndef MATCC_QUAD_LIST_H
#define MATCC_QUAD_LIST_H

#include <stdio.h>
#include "quad.h"

struct quad_list {
    struct quad *quad;
    struct quad_list *successor;
};

extern struct quad_list *quad_list;

struct quad_list *quad_list_new(struct quad *quad);
void quad_list_push(struct quad_list **dest, struct quad *quad);
void quad_list_concat(struct quad_list **dest, int sources_count, ...);
void quad_list_complete(struct quad_list *list, struct symbol *label);
void quad_list_print(FILE *f, struct quad_list *list);
void quad_list_delete(struct quad_list *list);

#endif //MATCC_QUAD_LIST_H
