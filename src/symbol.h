#ifndef MATCC_SYMBOL_H
#define MATCC_SYMBOL_H

#include <stdbool.h>
#include <stddef.h>
#include "matrix.h"

enum symbol_type {
    INT,
    FLOAT,
    STRING,
    MATRIX
};

union symbol_initial_value {
    int intval;
    float floatval;
    char *stringval;
    struct matrix *matrixval;
};

struct symbol {
    char *name;
    enum symbol_type type;
    union symbol_initial_value initial_value;
    bool constant;
    // Used only when symbol is a matrix
    struct matrix_size matrix_size;
    // Wether we access this symbol by adress (we then dont load its value into registers)
    bool by_adress;

    int start_point;
    int end_point;
    int life_time;
    size_t size;
    int affected_register;
    int stack_location;
};

#define POINT_INIT (-1)
#define REGISTER_NONE (-1)

struct symbol *symbol_new(char *name, enum symbol_type type);
struct symbol *symbol_new_temp(enum symbol_type type);
struct symbol *symbol_new_const(enum symbol_type type, union symbol_initial_value value);
void symbol_delete(struct symbol *s);

const char *SYMBOL_TYPE_STR[4];

#endif //MATCC_SYMBOL_H
