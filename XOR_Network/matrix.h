#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <err.h>

typedef struct Matrix
{
	size_t row;
	size_t col;
	size_t dataLen;
	double *data;
} Matrix;

void matrix_init(Matrix *m, size_t row, size_t col);
void matrix_free(Matrix *m);
void matrix_free_data(Matrix *m);

void matrix_identity(Matrix *m, size_t size);
void matrix_set(Matrix *m, size_t row, size_t col, double value);


Matrix *matrix_add(Matrix *m1, Matrix *m2);
void matrix_add_in_place(Matrix *m1, Matrix *m2);

Matrix *matrix_sub(Matrix *m1, Matrix *m2);
void matrix_sub_in_place(Matrix *m1, Matrix *m2);

Matrix *matrix_scalar_add(Matrix *m, double scalar);
void matrix_scalar_add_in_place(Matrix *m, double scalar);

Matrix *matrix_scalar_mul(Matrix *m, double scalar);
void matrix_scalar_mul_in_place(Matrix *m, double scalar);

Matrix *matrix_dot_product(Matrix *m1, Matrix *m2);
Matrix *matrix_transpose(Matrix *m);


void matrix_print(Matrix *m);
int matrix_equals(Matrix *m1, Matrix *m2);
