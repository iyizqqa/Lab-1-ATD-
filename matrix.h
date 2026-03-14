#pragma once

#include "types.h"

typedef struct _matrix
{
	size_t size;
	void *data;
	const FieldInfo *info;
} Matrix;

int init_matrix(Matrix *m, size_t n, const FieldInfo *info);
void clear_matrix(Matrix *m);
void *get_element(Matrix *m, size_t row, size_t col);
const void *get_element_const(const Matrix *m, size_t row, size_t col);

Matrix add_matrices(const Matrix *a, const Matrix *b);
Matrix multiply_matrices(const Matrix *a, const Matrix *b);
Matrix scalar_multiply_matrix(const Matrix *a, const void *scalar);

void input_matrix(Matrix *m);
void output_matrix(const Matrix *m);
