#ifndef MATCC_INSTRUCTION_H
#define MATCC_INSTRUCTION_H

#include <symbol.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

struct instruction {
    char *opcode;
    char *op1;
    char *op2;
    char *op3;
    bool is_label;
};

struct instruction_list {
    struct instruction *i;
    struct instruction_list *successor;
};

struct instruction *instruction_new(char *opcode, char *op1, char *op2, char *op3);
struct instruction *instruction_new_label(char *label, char *comment);
struct instruction_list *instruction_list_new(struct instruction *instruction);
void instruction_list_push(struct instruction_list **list, struct instruction *instruction);
void instruction_list_print(FILE *f, struct instruction_list *list);
void instruction_list_delete(struct instruction_list *list);

#endif //MATCC_INSTRUCTION_H
