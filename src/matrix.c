#include <parsing/actions.h>

struct matrix *matrix_new() {
    return safe_calloc(1, sizeof(struct matrix));
}

struct matrix *matrix_new_with_value(float value) {
    struct matrix *matrix = matrix_new();
    matrix_append_value(matrix, value);
    return matrix;
}

void matrix_append_value(struct matrix *matrix, float value) {
    if(matrix->height > 1) {
        abort_parsing("cannot append value to multi-dimensional matrix");
    }
    else if(matrix->height == 0) {
        // Adding value to an empty matrix
        matrix->height = 1;
        matrix->width = 1;
        matrix->values = safe_malloc(matrix->height * matrix->width * sizeof(float));
        matrix->values[0] = value;
    }
    else {
        matrix->width++;
        matrix->values = safe_realloc(matrix->values, matrix->height * matrix->width * sizeof(float));
        matrix->values[matrix->width-1] = value;
    }
}

void matrix_append_row(struct matrix *matrix, struct matrix *row) {
    if(matrix->width != row->width) {
        abort_parsing("trying to append row of size %zu to a matrix with rows of size %zu", row->width, matrix->width);
    }
    else if(row->height != 1) {
        abort_parsing("row height must be 1, but it is %d", row->height);
    }
    else {
        matrix->values = safe_realloc(matrix->values, (matrix->height + row->height) * matrix->width * sizeof(float));
        memcpy(&matrix->values[matrix->height * matrix->width], &row->values[0], row->width * sizeof(float));
        matrix->height++;
    }
}

void matrix_delete(struct matrix *matrix) {
    safe_free(matrix->values);
    safe_free(matrix);
}
