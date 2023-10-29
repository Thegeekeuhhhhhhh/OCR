#include "matrix.h"

void matrix_init(Matrix *m, size_t row, size_t col)
{
	/*
	Initialize a matrix its values to 0
	*/
	if (row < 1 || col < 1)
	{
		errx(1, "The matrix needs to have at least 1 row and 1 column to exist [Matrix lib]");
	}
	m->row = row;
	m->col = col;
	m->data = malloc(row * col * sizeof(double));
	m->dataLen = row * col;
	for (size_t i = 0; i < m->dataLen; i++)
	{
		m->data[i] = 0.0f;
	}
}

void matrix_free_data(Matrix *m)
{
	free(m->data);
}

void matrix_identity(Matrix *m, size_t size)
{
	/*
	Compute the identity matrix. e.g. matrix_identity(m, 2) => m = I_2
	*/
	if (size < 1)
	{
		errx(1, "Size must be > 1 for identity function [Matrix lib]");
	}
	matrix_free_data(m);
	matrix_init(m, size, size);
	for (size_t i = 0; i < size; i++)
	{
		m->data[i*size + i] = 1.0f; // Set the diagonals values to 1
	}
}

void matrix_set(Matrix *m, size_t row, size_t col, double value)
{
	m->data[row*(m->col) + col] = value;
}

double matrix_get(Matrix *m, size_t row, size_t col)
{
	return m->data[row*(m->col) + col];
}

Matrix *matrix_add(Matrix *m1, Matrix *m2)
{
	/*
	Adds 2 matrixes, return the resultant matrix
	*/
	if (m1->row != m2->row || m1->col != m2->col)
	{
		errx(1, "Tried to add matrixes with different dimensions [Matrix lib] :\nm1->row = %lu !\nm1->col = %lu !\nm2->row = %lu !\nm2->col = %lu !\n", m1->row, m1->col, m2->row, m2->col);
	}
	else
	{
		Matrix *result = malloc(sizeof(Matrix));
		matrix_init(result, m1->row, m1->col);
		for (size_t i = 0; i < result->row; i++)
		{
			for (size_t j = 0; j < result->col; j++)
			{
				double temp = matrix_get(m1, i, j) + matrix_get(m2, i, j);
				matrix_set(result, i, j, temp);
			}
		}
		return result;
	}
}

void matrix_add_in_place(Matrix *m1, Matrix *m2)
{
	/*
	Adds 2 matrixes, result is stored in m1 (Old value of m1 will be lost)
	*/
	if (m1->row != m2->row || m1->col != m2->col)
	{
		errx(1, "Tried to add matrixes with different dimensions [Matrix lib] :\nm1->row = %lu !\nm1->col = %lu !\nm2->row = %lu !\nm2->col = %lu !\n", m1->row, m1->col, m2->row, m2->col);
	}
	else
	{
		for (size_t i = 0; i < m1->row; i++)
		{
			for (size_t j = 0; j < m1->col; j++)
			{
				double temp = matrix_get(m1, i, j) + matrix_get(m2, i, j);
				matrix_set(m1, i, j, temp);
			}
		}
	}
}


Matrix *matrix_sub(Matrix *m1, Matrix *m2)
{
	/*
	Subtracts 2 matrixes, return the resultant matrix
	*/
	if (m1->row != m2->row || m1->col != m2->col)
	{
		errx(1, "Tried to add matrixes with different dimensions [Matrix lib] :\nm1->row = %lu !\nm1->col = %lu !\nm2->row = %lu !\nm2->col = %lu !\n", m1->row, m1->col, m2->row, m2->col);
	}
	else
	{
		Matrix *result = malloc(sizeof(Matrix));
		matrix_init(result, m1->row, m1->col);
		for (size_t i = 0; i < result->row; i++)
		{
			for (size_t j = 0; j < result->col; j++)
			{
				double temp = matrix_get(m1, i, j) - matrix_get(m2, i, j);
				matrix_set(result, i, j, temp);
			}
		}
		return result;
	}
}

void matrix_sub_in_place(Matrix *m1, Matrix *m2)
{
	/*
	Subtracts 2 matrixes, result is stored in m1 (Old value of m1 will be lost)
	*/
	if (m1->row != m2->row || m1->col != m2->col)
	{
		errx(1, "Tried to add matrixes with different dimensions [Matrix lib] :\nm1->row = %lu !\nm1->col = %lu !\nm2->row = %lu !\nm2->col = %lu !\n", m1->row, m1->col, m2->row, m2->col);
	}
	else
	{
		for (size_t i = 0; i < m1->row; i++)
		{
			for (size_t j = 0; j < m1->col; j++)
			{
				double temp = matrix_get(m1, i, j) - matrix_get(m2, i, j);
				matrix_set(m1, i, j, temp);
			}
		}
	}
}


Matrix *matrix_scalar_add(Matrix *m, double scalar);
void matrix_scalar_add_in_place(Matrix *m, double scalar);

Matrix *matrix_scalar_mul(Matrix *m, double scalar);
void matrix_scalar_mul_in_place(Matrix *m, double scalar);

Matrix *matrix_dot_product(Matrix *m1, Matrix *m2);
Matrix *matrix_transpose(Matrix *m);


void matrix_print(Matrix *m);
int matrix_equals(Matrix *m1, Matrix *m2);


int main()
{
	return 0;
}
