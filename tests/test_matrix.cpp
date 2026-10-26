/**
 * @file test_matrix.cpp
 * @brief Модульные тесты библиотеки "Калькулятор матриц" (ЛР-3, вариант 9).
 * @author Студент
 * @version 1.0
 *
 * Покрывают:
 *   - базовые операции (создание, сложение, умножение, освобождение);
 *   - вспомогательные функции (matrix_from_array);
 *   - индивидуальное задание (вариант 9) — поэлементное умножение;
 *   - граничные случаи (1x1, nullptr, нулевые размеры);
 *   - обработку ошибок (несовпадение размеров → std::invalid_argument).
 *
 * Фреймворк: Google Test (подключается через FetchContent в CMake).
 */

#include <gtest/gtest.h>

#include <stdexcept>

#include "matrix.h"

/* ================================================================
 * 1. БАЗОВЫЕ ОПЕРАЦИИ
 * ================================================================ */

// 1.1. Создание матрицы — нормальный случай
TEST(MatrixTest, CreateMatrix_NormalCase) {
    matrix::Matrix A = matrix::create_matrix(3, 4);

    EXPECT_EQ(A.rows, 3);
    EXPECT_EQ(A.cols, 4);
    ASSERT_NE(A.data, nullptr);

    // Проверка инициализации нулями
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_DOUBLE_EQ(A.data[i][j], 0.0);
        }
    }

    matrix::free_matrix(A);
}

// 1.2. Сложение матриц — нормальный случай
TEST(MatrixTest, Addition_NormalCase) {
    matrix::Matrix A = matrix::create_matrix(2, 2);
    matrix::Matrix B = matrix::create_matrix(2, 2);

    A.data[0][0] = 1; A.data[0][1] = 2;
    A.data[1][0] = 3; A.data[1][1] = 4;

    B.data[0][0] = 5; B.data[0][1] = 6;
    B.data[1][0] = 7; B.data[1][1] = 8;

    matrix::Matrix C = matrix::matrix_add(A, B);

    EXPECT_DOUBLE_EQ(C.data[0][0], 6.0);
    EXPECT_DOUBLE_EQ(C.data[0][1], 8.0);
    EXPECT_DOUBLE_EQ(C.data[1][0], 10.0);
    EXPECT_DOUBLE_EQ(C.data[1][1], 12.0);

    matrix::free_matrix(A);
    matrix::free_matrix(B);
    matrix::free_matrix(C);
}

// 1.3. Матричное умножение — нормальный случай
TEST(MatrixTest, Multiplication_NormalCase) {
    matrix::Matrix A = matrix::create_matrix(2, 2);
    matrix::Matrix B = matrix::create_matrix(2, 2);

    A.data[0][0] = 1; A.data[0][1] = 2;
    A.data[1][0] = 3; A.data[1][1] = 4;

    B.data[0][0] = 5; B.data[0][1] = 6;
    B.data[1][0] = 7; B.data[1][1] = 8;

    matrix::Matrix C = matrix::matrix_multiply(A, B);

    EXPECT_DOUBLE_EQ(C.data[0][0], 19.0);
    EXPECT_DOUBLE_EQ(C.data[0][1], 22.0);
    EXPECT_DOUBLE_EQ(C.data[1][0], 43.0);
    EXPECT_DOUBLE_EQ(C.data[1][1], 50.0);

    matrix::free_matrix(A);
    matrix::free_matrix(B);
    matrix::free_matrix(C);
}

// 1.4. Освобождение памяти — безопасный вызов
TEST(MatrixTest, FreeMatrix_NormalCase) {
    matrix::Matrix A = matrix::create_matrix(3, 3);
    EXPECT_NO_THROW(matrix::free_matrix(A));
}

/* ================================================================
 * 2. ГРАНИЧНЫЕ СЛУЧАИ
 * ================================================================ */

// 2.1. Минимальный размер матрицы — 1×1
TEST(MatrixTest, CreateMatrix_MinimumSize) {
    matrix::Matrix A = matrix::create_matrix(1, 1);

    EXPECT_EQ(A.rows, 1);
    EXPECT_EQ(A.cols, 1);
    EXPECT_DOUBLE_EQ(A.data[0][0], 0.0);

    matrix::free_matrix(A);
}

// 2.2. Умножение вектор-строка × вектор-столбец → матрица 1×1
TEST(MatrixTest, Multiplication_RowTimesColumn_1x1) {
    matrix::Matrix A = matrix::create_matrix(1, 3);  // [1, 2, 3]
    matrix::Matrix B = matrix::create_matrix(3, 1);  // [4; 5; 6]

    A.data[0][0] = 1; A.data[0][1] = 2; A.data[0][2] = 3;
    B.data[0][0] = 4; B.data[1][0] = 5; B.data[2][0] = 6;

    matrix::Matrix C = matrix::matrix_multiply(A, B);

    EXPECT_EQ(C.rows, 1);
    EXPECT_EQ(C.cols, 1);
    EXPECT_DOUBLE_EQ(C.data[0][0], 32.0);  // 1*4 + 2*5 + 3*6 = 32

    matrix::free_matrix(A);
    matrix::free_matrix(B);
    matrix::free_matrix(C);
}

// 2.3. Создание матрицы из массива
TEST(MatrixTest, FromArray_NormalCase) {
    double arr[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    matrix::Matrix M = matrix::matrix_from_array(arr, 2, 3);

    EXPECT_EQ(M.rows, 2);
    EXPECT_EQ(M.cols, 3);
    EXPECT_DOUBLE_EQ(M.data[0][0], 1.0);
    EXPECT_DOUBLE_EQ(M.data[0][2], 3.0);
    EXPECT_DOUBLE_EQ(M.data[1][0], 4.0);
    EXPECT_DOUBLE_EQ(M.data[1][2], 6.0);

    matrix::free_matrix(M);
}

// 2.4. Двойное освобождение пустой матрицы — безопасно
TEST(MatrixTest, FreeMatrix_EmptyIsSafe) {
    matrix::Matrix empty{nullptr, 0, 0};
    EXPECT_NO_THROW(matrix::free_matrix(empty));
    EXPECT_NO_THROW(matrix::free_matrix(empty));
}

/* ================================================================
 * 3. НЕКОРРЕКТНЫЕ СЛУЧАИ (ошибки)
 * ================================================================ */

// 3.1. Создание матрицы с нулевыми размерами
TEST(MatrixTest, CreateMatrix_ZeroRows) {
    EXPECT_THROW(matrix::create_matrix(0, 3), std::invalid_argument);
}

// 3.2. Создание матрицы с отрицательным размером
TEST(MatrixTest, CreateMatrix_NegativeCols) {
    EXPECT_THROW(matrix::create_matrix(3, -1), std::invalid_argument);
}

// 3.3. Сложение матриц разных размеров
TEST(MatrixTest, Addition_SizeMismatch) {
    matrix::Matrix A = matrix::create_matrix(2, 2);
    matrix::Matrix B = matrix::create_matrix(3, 3);

    EXPECT_THROW(matrix::matrix_add(A, B), std::invalid_argument);

    matrix::free_matrix(A);
    matrix::free_matrix(B);
}

// 3.4. Умножение матриц с несовместимыми размерами
TEST(MatrixTest, Multiplication_SizeMismatch) {
    matrix::Matrix A = matrix::create_matrix(2, 3);
    matrix::Matrix B = matrix::create_matrix(2, 2);

    EXPECT_THROW(matrix::matrix_multiply(A, B), std::invalid_argument);

    matrix::free_matrix(A);
    matrix::free_matrix(B);
}

// 3.5. matrix_from_array с nullptr
TEST(MatrixTest, FromArray_NullData) {
    EXPECT_THROW(matrix::matrix_from_array(nullptr, 2, 3), std::invalid_argument);
}

/* ================================================================
 * 4. ВАРИАНТ 9: ПОЭЛЕМЕНТНОЕ УМНОЖЕНИЕ
 * ================================================================ */

// 4.1. Нормальный случай — матрицы 2×3
TEST(ElementwiseTest, NormalCase) {
    // A = [[1, 2, 3], [4, 5, 6]]
    // B = [[2, 3, 4], [5, 6, 7]]
    // C = [[2, 6, 12], [20, 30, 42]]
    double a[] = {1, 2, 3, 4, 5, 6};
    double b[] = {2, 3, 4, 5, 6, 7};

    matrix::Matrix A = matrix::matrix_from_array(a, 2, 3);
    matrix::Matrix B = matrix::matrix_from_array(b, 2, 3);

    matrix::Matrix C = matrix::matrix_multiply_elementwise(A, B);

    EXPECT_EQ(C.rows, 2);
    EXPECT_EQ(C.cols, 3);
    EXPECT_DOUBLE_EQ(C.data[0][0], 2.0);
    EXPECT_DOUBLE_EQ(C.data[0][1], 6.0);
    EXPECT_DOUBLE_EQ(C.data[0][2], 12.0);
    EXPECT_DOUBLE_EQ(C.data[1][0], 20.0);
    EXPECT_DOUBLE_EQ(C.data[1][1], 30.0);
    EXPECT_DOUBLE_EQ(C.data[1][2], 42.0);

    matrix::free_matrix(A);
    matrix::free_matrix(B);
    matrix::free_matrix(C);
}

// 4.2. Матрица 1×1 — граничный случай
TEST(ElementwiseTest, SingleElement) {
    matrix::Matrix A = matrix::create_matrix(1, 1);
    matrix::Matrix B = matrix::create_matrix(1, 1);

    A.data[0][0] = 7.0;
    B.data[0][0] = 6.0;

    matrix::Matrix C = matrix::matrix_multiply_elementwise(A, B);

    EXPECT_DOUBLE_EQ(C.data[0][0], 42.0);

    matrix::free_matrix(A);
    matrix::free_matrix(B);
    matrix::free_matrix(C);
}

// 4.3. Наличие нулей в матрицах
TEST(ElementwiseTest, WithZeros) {
    double a[] = {0, 5, 0, 3};
    double b[] = {9, 0, 7, 4};

    matrix::Matrix A = matrix::matrix_from_array(a, 2, 2);
    matrix::Matrix B = matrix::matrix_from_array(b, 2, 2);

    matrix::Matrix C = matrix::matrix_multiply_elementwise(A, B);

    EXPECT_DOUBLE_EQ(C.data[0][0], 0.0);   // 0 * 9
    EXPECT_DOUBLE_EQ(C.data[0][1], 0.0);   // 5 * 0
    EXPECT_DOUBLE_EQ(C.data[1][0], 0.0);   // 0 * 7
    EXPECT_DOUBLE_EQ(C.data[1][1], 12.0);  // 3 * 4

    matrix::free_matrix(A);
    matrix::free_matrix(B);
    matrix::free_matrix(C);
}

// 4.4. Отрицательные числа
TEST(ElementwiseTest, WithNegativeNumbers) {
    double a[] = {-1, 2, 3, -4};
    double b[] = {5, -6, -7, 8};

    matrix::Matrix A = matrix::matrix_from_array(a, 2, 2);
    matrix::Matrix B = matrix::matrix_from_array(b, 2, 2);

    matrix::Matrix C = matrix::matrix_multiply_elementwise(A, B);

    EXPECT_DOUBLE_EQ(C.data[0][0], -5.0);
    EXPECT_DOUBLE_EQ(C.data[0][1], -12.0);
    EXPECT_DOUBLE_EQ(C.data[1][0], -21.0);
    EXPECT_DOUBLE_EQ(C.data[1][1], -32.0);

    matrix::free_matrix(A);
    matrix::free_matrix(B);
    matrix::free_matrix(C);
}

// 4.5. Ошибка: несовпадение размеров
TEST(ElementwiseTest, SizeMismatch) {
    matrix::Matrix A = matrix::create_matrix(2, 3);
    matrix::Matrix B = matrix::create_matrix(3, 2);

    EXPECT_THROW(matrix::matrix_multiply_elementwise(A, B), std::invalid_argument);

    matrix::free_matrix(A);
    matrix::free_matrix(B);
}