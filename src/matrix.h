#ifndef MATCC_MATRIX_H
#define MATCC_MATRIX_H

#include <stddef.h>

struct matrix {
    size_t height;
    size_t width;
    float *values;
};

struct matrix_size {
    size_t height;
    size_t width;
};

struct matrix *matrix_new();
struct matrix *matrix_new_with_value(float value);
void matrix_append_value(struct matrix *matrix, float value);
void matrix_append_row(struct matrix *matrix, struct matrix *row);

void matrix_delete(struct matrix *matrix);

#endif //MATCC_MATRIX_H
