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


Matrix *matrix_scalar_add(Matrix *m, double scalar)
{
    /*
       Adds a scalar to a matrix, return the resultant matrix
       */
    Matrix *result = malloc(sizeof(Matrix));
    matrix_init(result, m->row, m->col);
    for (size_t i = 0; i < result->row; i++)
    {
        for (size_t j = 0; j < result->col; j++)
        {
            double temp = matrix_get(m, i, j) + scalar;
            matrix_set(result, i, j, temp);
        }
    }
    return result;
}



void matrix_scalar_add_in_place(Matrix *m, double scalar)
{
    /*
       Adds a scalar to a matrix, the result is stored in m, (Old m will be lost)
       */
    for (size_t i = 0; i < m->row; i++)
    {
        for (size_t j = 0; j < m->col; j++)
        {
            double temp = matrix_get(m, i, j) + scalar;
            matrix_set(m, i, j, temp);
        }
    }
}


Matrix *matrix_scalar_mul(Matrix *m, double scalar)
{
    /*
       Multiply a matrix with a scalar, return the resultant matrix
       */
    Matrix *result = malloc(sizeof(Matrix));
    matrix_init(result, m->row, m->col);
    for (size_t i = 0; i < result->row; i++)
    {
        for (size_t j = 0; j < result->col; j++)
        {
            double temp = matrix_get(m, i, j) * scalar;
            matrix_set(result, i, j, temp);
        }
    }
    return result;
}

void matrix_scalar_mul_in_place(Matrix *m, double scalar)
{
    /*
       Multiply a matrix with a scalar, the result is stored in m (Old m will be lost)
       */

    for (size_t i = 0; i < m->row; i++)
    {
        for (size_t j = 0; j < m->col; j++)
        {
            double temp = matrix_get(m, i, j) * scalar;
            matrix_set(m, i, j, temp);
        }
    }
}


Matrix *matrix_mul(Matrix *m1, Matrix *m2)
{
    /*
       Return the resultat matrix of an HADAMARD product
       We just multiply each elements by its corresponding
       */
    Matrix *result = malloc(sizeof(Matrix));
    matrix_init(result, m1->row, m1->col);
    for (size_t i = 0; i < result->row; i++)
    {
        for (size_t j = 0; j < result->col; j++)
        {
            double temp = matrix_get(m1, i, j) * matrix_get(m2, i, j);
            matrix_set(result, i, j, temp);
        }
    }
    return result;
}

void matrix_mul_in_place(Matrix *m1, Matrix *m2)
{
    /*
       Multiply a matrix by another using HADAMARD product
       the result will be stored in m1 (Old m1 will be lost)
       */
    for (size_t i = 0; i < m1->row; i++)
    {
        for (size_t j = 0; j < m1->col; j++)
        {
            double temp = matrix_get(m1, i, j) * matrix_get(m2, i, j);
            matrix_set(m1, i, j, temp);
        }
    }
}


Matrix *matrix_dot_product(Matrix *m1, Matrix *m2)
{
    /*
       Return the resultant matrix of the dot product of m1 * m2
       */
    if (m1->col != m2->row)
    {
        errx(1, "Tried to multiply 2 matrices with unappropriate dimensions [Matrix lib] :\nm1->row = %lu !\nm1->col = %lu !\nm2->row = %lu !\nm2->col = %lu !\n", m1->row, m1->col, m2->row, m2->col);
    }
    else
    {
        Matrix *result = malloc(m1->row * m2->col * sizeof(double));
        matrix_init(result, m1->row, m2->col);
        for (size_t i = 0; i < result->row; i++)
        {
            for (size_t j = 0; j < result->col; j++)
            {
                double temp = 0;
                for (size_t k = 0; k < m1->col; k++)
                {
                    temp += matrix_get(m1, i, k) * matrix_get(m2, k, j); // Thank you S3 Midterm :)
                }
                matrix_set(result, i, j, temp);
            }
        }
        return result;
    }
}

Matrix *matrix_transpose(Matrix *m)
{
    /*
       Function used to transpose a matrix m, return the resultant matrix in a new pointer
       (The function is not optimized)
       */
    Matrix *result = malloc(m->row * m->col * sizeof(double));
    matrix_init(result, m->row, m->col);
    for (size_t i = 0; i < result->row; i++)
    {
        for (size_t j = 0; j < result->col; j++)
        {
            double temp = matrix_get(m, i, j);
            matrix_set(result, j, i, temp);
        }
    }
    return result;
}

Matrix *matrix_apply_function(Matrix *m, double (*f)(double))
{
    /*
       Apply a function to every elements of the matrix m, return the resultant matrix in a new pointer
       */
    Matrix *result = malloc(m->row * m->col * sizeof(double));
    matrix_init(result, m->row, m->col);
    for (size_t i = 0; i < result->row; i++)
    {
        for (size_t j = 0; j < result->col; j++)
        {
            double temp = f(matrix_get(m, i, j));
            matrix_set(result, i, j, temp);
        }
    }
    return result;
}

void matrix_apply_function_in_place(Matrix *m, double (*f)(double))
{
    /*
       Apply a function to every elements of the matrix m in place (Old m will be lost)
       The function is a function that takes a double and return a double, e.g. Sigmoid function
       */
    for (size_t i = 0; i < m->row; i++)
    {
        for (size_t j = 0; j < m->col; j++)
        {
            double temp = f(matrix_get(m, i, j));
            matrix_set(m, i, j, temp);
        }
    }
}

void matrix_print(Matrix *m)
{
    /*
       Prints the matrix m in a beautiful way
       */
    for (size_t i = 0; i < m->row; i++)
    {
        printf("|");
        for (size_t j = 0; j < m->col; j++)
        {
            printf("%2.3lf\t", matrix_get(m, i, j));
        }
        printf("|\n");
    }
}

void separator()
{
    /*
       Prints a separator (Useful when many matrixes are printed)
       */
    printf("\n%s\n\n", "===============================================================================");
}	

int matrix_equals(Matrix *m1, Matrix *m2)
{
    /*
       Tests if m1 is equal to m2, return 1 if m1 == m2, 0 else
       */
    if (m1->row != m2->row || m1->col != m2->col)
    {
        return 0;
    }
    for (size_t i = 0; i < m1->row; i++)
    {
        for (size_t j = 0; j < m1->col; j++)
        {
            if (matrix_get(m1, i, j) != matrix_get(m2, i, j))
            {
                return 0;
            }
        }
    }
    return 1;
}

/*
   int main()
   {
   Matrix *test1 = malloc(4*4*sizeof(double));
   matrix_init(test1, 4, 4);
   matrix_print(test1);
   separator();
   matrix_set(test1, 1, 3, 1.0f);
   matrix_print(test1);
   separator();
   matrix_print(matrix_transpose(test1));
   return 0;
   }
   */
