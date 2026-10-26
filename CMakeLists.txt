cmake_minimum_required(VERSION 3.20)
project(MatrixCalculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

include_directories(include)

add_executable(matrix_calculator
    src/main.cpp
    src/matrix.cpp
)

enable_testing()
add_subdirectory(tests)