#pragma once

namespace matrix {
struct Matrix {
 double** data; // двумерный массив данных
 int rows; // количество строк
 int cols; // количество столбцов
};

Matrix create_matrix(int rows, int cols);

void free_matrix(Matrix m);

Matrix matrix_add(Matrix a, Matrix b);

Matrix matrix_multiply(Matrix a, Matrix b);

Matrix matrix_from_array(double* data, int rows, int cols);

void print_matrix(Matrix m);

Matrix matrix_multiply_elementwise(Matrix a, Matrix b);

}  