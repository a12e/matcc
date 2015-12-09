#ifndef MATCC_INSTRUCTION_H
#define MATCC_INSTRUCTION_H

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

struct instruction {
    char *opcode;
    char *op1;
    char *op2;
    char *op3;
};

struct instruction_list {
    struct instruction *i;
    struct instruction_list *successor;
};

#endif //MATCC_INSTRUCTION_H
