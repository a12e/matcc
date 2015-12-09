#ifndef MATCC_SYMBOL_H
#define MATCC_SYMBOL_H

#include <stdbool.h>

enum symbol_type {
    INT,
    FLOAT,
    MATRIX
};

union symbol_initial_value {
    int intval;
    float floatval;
};

struct symbol {
    char *name;
    enum symbol_type type;
    union symbol_initial_value initial_value;
    bool temporary;
    bool constant;

    int start_point;
    int end_point;
    int life_time;
    int affected_register;
    int stack_location;
};

#define POINT_INIT (-1)
#define REGISTER_NONE (-1)

struct symbol *symbol_new(char *name, enum symbol_type type);
struct symbol *symbol_new_temp(enum symbol_type type);
struct symbol *symbol_new_const(enum symbol_type type, union symbol_initial_value value);
void symbol_delete(struct symbol *s);

const char *SYMBOL_TYPE_STR[3];

#endif //MATCC_SYMBOL_H
