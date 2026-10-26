include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(matrix_tests
    test_matrix.cpp
    ${CMAKE_SOURCE_DIR}/src/matrix.cpp
)

target_link_libraries(matrix_tests PRIVATE gtest_main)
target_include_directories(matrix_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(matrix_tests)