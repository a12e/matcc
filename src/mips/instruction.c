#include <utility.h>
#include "instruction.h"


struct instruction *instruction_new(char *opcode, char *op1, char *op2, char *op3) {
    struct instruction *instruction = safe_malloc(sizeof(struct instruction));
    instruction->opcode = safe_strdup(opcode);
    instruction->op1 = safe_strdup(op1);
    instruction->op2 = safe_strdup(op2);
    instruction->op3 = safe_strdup(op3);
    instruction->is_label = false;
    return instruction;
}

struct instruction *instruction_new_label(char *label, char *comment) {
    struct instruction *i = instruction_new(label, comment, NULL, NULL);
    i->is_label = true;
    return i;
}

struct instruction_list *instruction_list_new(struct instruction *instruction) {
    struct instruction_list *list = safe_malloc(sizeof(struct instruction_list));
    list->i = instruction;
    list->successor = NULL;
    return list;
}

void instruction_list_push(struct instruction_list **list, struct instruction *instruction) {
    if(*list == NULL) {
        *list = instruction_list_new(instruction);
    }
    else {
        struct instruction_list *l = *list;
        for(; l->successor != NULL; l = l->successor);
        l->successor = instruction_list_new(instruction);
    }
}

void instruction_list_print(FILE *f, struct instruction_list *list) {
    for(struct instruction_list *l = list; l != NULL; l = l->successor) {
        if(l->i->is_label)
            fprintf(f, "%s: # %s\n",
                    l->i->opcode,
                    l->i->op1 ? l->i->op1 : ""
            );
        else
            fprintf(f, "\t%s %s %s %s\n",
                    l->i->opcode,
                    l->i->op1 ? l->i->op1 : "",
                    l->i->op2 ? l->i->op2 : "",
                    l->i->op3 ? l->i->op3 : ""
            );

    }
}

void instruction_delete(struct instruction *i) {
    safe_free(i->opcode);
    safe_free(i->op1);
    safe_free(i->op2);
    safe_free(i->op3);
    safe_free(i);
}

void instruction_list_delete(struct instruction_list *list) {
    for(struct instruction_list *l = list; l != NULL; l = l->successor) {
        //instruction_delete(list->i);
        safe_free(l);
    }
}
