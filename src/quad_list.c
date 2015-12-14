#include <stdarg.h>
#include "quad_list.h"
#include "utility.h"
#include "symbol.h"

struct quad_list *quad_list = NULL;

struct quad_list *quad_list_new(struct quad *quad) {
    struct quad_list *quad_list = safe_malloc(sizeof(struct quad_list));
    quad_list->quad = quad;
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

struct quad_list *quad_list_concat(int sources_count, ...) {
    struct quad_list *result = NULL;
    va_list argptr;
    va_start(argptr, sources_count);

    for(int i = 0; i < sources_count; i++) {
        struct quad_list *src = va_arg(argptr, struct quad_list *);
        if (result == NULL) {
            result = src;
        }
        else {
            struct quad_list *l = result;
            for(; l->successor != NULL; l = l->successor);
            l->successor = src;
        }
    }

    va_end(argptr);
    return result;
}

void quad_list_complete(struct quad_list *list, struct symbol *label) {
    for(; list != NULL; list = list->successor) {
        list->quad->res = label;
    }
}

size_t quad_list_size(struct quad_list *list) {
    size_t size = 0;
    for(; list != NULL; list = list->successor) {
        size++;
    }
    return size;
}

void quad_list_print(FILE *f, struct quad_list *list) {
    fprintf(f, "(%zu quads)\n", quad_list_size(list));
    unsigned int quad_counter = 0;
    for(; list != NULL; list = list->successor) {
        fprintf(f, "%04u: ", quad_counter++);
        quad_print(f, list->quad);
    }
}

void quad_list_delete(struct quad_list *list) {
    quad_delete(list->quad);
    if(list->successor) quad_list_delete(list->successor);
    safe_free(list);
}

