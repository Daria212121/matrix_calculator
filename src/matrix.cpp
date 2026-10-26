#include "../include/matrix.h"

#include <iomanip>
#include <iostream>
#include <new>
#include <stdexcept>


namespace matrix {

namespace {

    bool is_valid_matrix(const Matrix& m) {
        return m.data != nullptr && m.rows > 0 && m.cols > 0;
    }

}  // namespace

/*create_matrix*/

/**
 * @brief Создание новой матрицы заданного размера, инициализированной нулями.
 *
 * @param rows Количество строк (должно быть > 0).
 * @param cols Количество столбцов (должно быть > 0).
 * @return Matrix Вновь созданная инициализированная матрица.
 * @exception std::invalid_argument Если rows <= 0 или cols <= 0.
 * @exception std::bad_alloc Если не удалось выделить память.
 *
 * @note Время выполнения: O(rows * cols).
 * @see free_matrix()
 */
Matrix create_matrix(int rows, int cols) {
    // 1. Проверка корректности размеров
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Matrix dimensions must be positive integers");
    }

    Matrix mat{};
    mat.rows = rows;
    mat.cols = cols;
    mat.data = nullptr;

    // 2. Выделение памяти для массива указателей на строки
    mat.data = new (std::nothrow) double*[rows];
    if (mat.data == nullptr) {
        throw std::bad_alloc();
    }

    // 3. Выделение памяти для каждой строки и инициализация нулями
    for (int i = 0; i < rows; ++i) {
        mat.data[i] = new (std::nothrow) double[cols]();  // () — инициализация нулями
        if (mat.data[i] == nullptr) {
            // При ошибке освобождаем уже выделенную память
            for (int j = 0; j < i; ++j) {
                delete[] mat.data[j];
            }
            delete[] mat.data;
            mat.data = nullptr;
            mat.rows = 0;
            mat.cols = 0;
            throw std::bad_alloc();
        }
    }

    return mat;
}

/**
 * @brief Освобождение памяти, занятой матрицей.
 *
 * @param m Матрица, память которой нужно освободить.
 *
 * @note Безопасно вызывать для уже освобождённой матрицы (защита от double free).
 * @warning После вызова m.data == nullptr, m.rows == 0, m.cols == 0.
 */
void free_matrix(Matrix m) {
    // 1. Проверка на nullptr — защита от двойного освобождения
    if (m.data == nullptr) {
        return;
    }

    // 2. Освобождение памяти каждой строки
    for (int i = 0; i < m.rows; ++i) {
        if (m.data[i] != nullptr) {
            delete[] m.data[i];
            m.data[i] = nullptr;
        }
    }

    // 3. Освобождение массива указателей
    delete[] m.data;

    // 4. Обнуление структуры (защита от use-after-free)
    m.data = nullptr;
    m.rows = 0;
    m.cols = 0;
}

/*matrix_add*/

/**
 * @brief Adds two matrices of equal dimensions.
 * @param a The first matrix.
 * @param b The second matrix.
 * @return A newly allocated matrix containing the sum.
 * @throws std::invalid_argument If matrix data is invalid or dimensions differ.
 */
Matrix matrix_add(Matrix a, Matrix b)
{
    if (a.data == nullptr || b.data == nullptr ||
        a.rows <= 0 || a.cols <= 0 ||
        b.rows <= 0 || b.cols <= 0) {
        throw std::invalid_argument("Invalid matrix");
    }

    if (a.rows != b.rows || a.cols != b.cols) {
        throw std::invalid_argument(
            "Matrices must have identical dimensions"
        );
    }

    for (int i = 0; i < a.rows; ++i) {
        if (a.data[i] == nullptr || b.data[i] == nullptr) {
            throw std::invalid_argument("Invalid matrix row");
        }
    }

    Matrix result{nullptr, a.rows, a.cols};
    result.data = new double*[result.rows]{};

    try {
        for (int i = 0; i < result.rows; ++i) {
            result.data[i] = new double[result.cols];

            for (int j = 0; j < result.cols; ++j) {
                result.data[i][j] =
                    a.data[i][j] + b.data[i][j];
            }
        }
    } catch (...) {
        for (int i = 0; i < result.rows; ++i) {
            delete[] result.data[i];
        }

        delete[] result.data;
        throw;
    }

    return result;
}

enum MatrixError {
    MATRIX_SUCCESS = 0,
    MATRIX_INVALID_ARGUMENT = 1,
    MATRIX_INCOMPATIBLE_DIMENSIONS = 2,
    MATRIX_ALLOCATION_ERROR = 3
};


/**
 * @brief Multiplies two matrices.
 * @param a The left matrix.
 * @param b The right matrix.
 * @return A new matrix containing the multiplication result.
 * @throws std::invalid_argument If matrix data is invalid or dimensions are incompatible.
 */
Matrix matrix_multiply(Matrix a, Matrix b)
{
    if (a.data == nullptr || b.data == nullptr ||
        a.rows <= 0 || a.cols <= 0 ||
        b.rows <= 0 || b.cols <= 0) {
        throw std::invalid_argument("Invalid matrix");
    }

    if (a.cols != b.rows) {
        throw std::invalid_argument(
            "Matrix dimensions are incompatible"
        );
    }

    for (int i = 0; i < a.rows; ++i) {
        if (a.data[i] == nullptr) {
            throw std::invalid_argument("Invalid matrix row");
        }
    }

    for (int i = 0; i < b.rows; ++i) {
        if (b.data[i] == nullptr) {
            throw std::invalid_argument("Invalid matrix row");
        }
    }

    Matrix result{nullptr, a.rows, b.cols};
    result.data = new double*[result.rows]();

    try {
        for (int i = 0; i < result.rows; ++i) {
            result.data[i] = new double[result.cols]();
        }

        for (int i = 0; i < a.rows; ++i) {
            for (int k = 0; k < a.cols; ++k) {
                const double aValue = a.data[i][k];

                for (int j = 0; j < b.cols; ++j) {
                    result.data[i][j] +=
                        aValue * b.data[k][j];
                }
            }
        }
    } catch (...) {
        for (int i = 0; i < result.rows; ++i) {
            delete[] result.data[i];
        }

        delete[] result.data;
        throw;
    }

    return result;
}

/**
 * @brief Освобождает память матрицы, созданной функцией matrix_multiply.
 * @param matrix Матрица, память которой необходимо освободить.
 * @return Ничего не возвращает.
 */
void matrix_free(Matrix matrix)
{
    if (matrix.data != nullptr) {
        for (int i = 0; i < matrix.rows; ++i) {
            delete[] matrix.data[i];
        }

        delete[] matrix.data;
    }
}
/*matrix_from_array*/

/**
 * @brief Создание матрицы из одномерного массива (заполнение по строкам).
 *
 * @param data Указатель на одномерный массив размера rows * cols.
 * @param rows Количество строк.
 * @param cols Количество столбцов.
 * @return Matrix Новая матрица.
 * @exception std::invalid_argument Если data == nullptr или размеры <= 0.
 */
Matrix matrix_from_array(double* data, int rows, int cols) {
    if (data == nullptr || rows <= 0 || cols <= 0) {
        throw std::invalid_argument("matrix_from_array: invalid arguments");
    }

    Matrix m = create_matrix(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            m.data[i][j] = data[i * cols + j];
        }
    }
    return m;
}

/*print_matrix*/

/**
 * @brief Вывод матрицы в stdout в читаемом формате.
 *
 * @param m Матрица для вывода.
 *
 * @note Для матриц больше 10×8 выводится сокращённый вид с многоточием.
 */
void print_matrix(Matrix m) {
    // 1. Проверка валидности матрицы
    if (!is_valid_matrix(m)) {
        std::cout << "[Empty matrix]" << std::endl;
        return;
    }

    // 2. Настройка формата вывода
    std::cout << std::fixed << std::setprecision(2);

    // 3. Проверка необходимости сокращения вывода
    const int MAX_ROWS_TO_SHOW = 10;
    const int MAX_COLS_TO_SHOW = 8;
    bool show_summary = (m.rows > MAX_ROWS_TO_SHOW || m.cols > MAX_COLS_TO_SHOW);

    int rows_to_show = show_summary ? MAX_ROWS_TO_SHOW : m.rows;
    int cols_to_show = show_summary ? MAX_COLS_TO_SHOW : m.cols;

    // 4. Вывод матрицы
    for (int i = 0; i < rows_to_show; ++i) {
        // Пропуск средних строк для больших матриц
        if (show_summary && i == MAX_ROWS_TO_SHOW / 2 && m.rows > MAX_ROWS_TO_SHOW) {
            std::cout << "[...";
            for (int j = 0; j < cols_to_show - 2; ++j) {
                std::cout << "      ";
            }
            std::cout << " ...]" << std::endl;
            continue;
        }

        std::cout << "[";
        for (int j = 0; j < cols_to_show; ++j) {
            // Пропуск средних столбцов для больших матриц
            if (show_summary && j == MAX_COLS_TO_SHOW / 2 && m.cols > MAX_COLS_TO_SHOW) {
                std::cout << "...";
                continue;
            }
            std::cout << std::setw(6) << m.data[i][j];
            if (j < cols_to_show - 1) {
                std::cout << " ";
            }
        }
        std::cout << "]";

        if (show_summary && m.cols > MAX_COLS_TO_SHOW) {
            std::cout << " ...";
        }
        std::cout << std::endl;
    }

    // 5. Добавление многоточия, если пропущены строки
    if (show_summary && m.rows > MAX_ROWS_TO_SHOW) {
        std::cout << "[...";
        int limit = (cols_to_show < m.cols) ? cols_to_show : m.cols;
        for (int j = 0; j < limit; ++j) {
            std::cout << "...";
        }
        std::cout << " ...]" << std::endl;
    }

    // 6. Информация о размере, если матрица большая
    if (show_summary) {
        std::cout << "Matrix " << m.rows << "x" << m.cols
                  << " (showing first " << rows_to_show << "x" << cols_to_show << ")"
                  << std::endl;
    }
}

/*ВАРИАНТ 9: поэлементное умножение*/

/**
 * @brief Performs element-wise multiplication of two matrices.
 * @param a The first matrix.
 * @param b The second matrix.
 * @return A new matrix containing the element-wise products.
 * @throws std::invalid_argument If the matrices have invalid or different dimensions.
 */
Matrix matrix_multiply_elementwise(Matrix a, Matrix b) {
    if (a.data == nullptr || b.data == nullptr ||
        a.rows <= 0 || a.cols <= 0 ||
        a.rows != b.rows || a.cols != b.cols) {
        throw std::invalid_argument(
            "Matrices must have identical valid dimensions"
        );
    }

    Matrix result{nullptr, a.rows, a.cols};
    result.data = new double*[result.rows]{};

    try {
        for (int i = 0; i < result.rows; ++i) {
            if (a.data[i] == nullptr || b.data[i] == nullptr) {
                throw std::invalid_argument("Invalid matrix row");
            }

            result.data[i] = new double[result.cols];

            for (int j = 0; j < result.cols; ++j) {
                result.data[i][j] = a.data[i][j] * b.data[i][j];
            }
        }
    } catch (...) {
        for (int i = 0; i < result.rows; ++i) {
            delete[] result.data[i];
        }

        delete[] result.data;
        throw;
    }

    return result;
}

}  // namespace matrix