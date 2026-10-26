#include "../include/matrix.h"

#include <iostream>

int main() {
    try {
        double dataA[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
        double dataB[] = {2.0, 3.0, 4.0, 5.0, 6.0, 7.0};

        matrix::Matrix A = matrix::matrix_from_array(dataA, 2, 3);
        matrix::Matrix B = matrix::matrix_from_array(dataB, 2, 3);

        std::cout << "=== Matrix A (2x3) ===" << std::endl;
        matrix::print_matrix(A);

        std::cout << "\n=== Matrix B (2x3) ===" << std::endl;
        matrix::print_matrix(B);

        matrix::Matrix C = matrix::matrix_multiply_elementwise(A, B);

        std::cout << "\n=== A .* B (поэлементное умножение) ===" << std::endl;
        matrix::print_matrix(C);

        matrix::free_matrix(A);
        matrix::free_matrix(B);
        matrix::free_matrix(C);

        std::cout << "\n[OK] Все операции выполнены успешно." << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return 1;
    }
}