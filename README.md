# Matrix Calculator (вариант 9)

Библиотека на C++17 для работы с матрицами.  
**Индивидуальное задание:** поэлементное (адамарово) умножение матриц.

## Описание задачи

Реализован «калькулятор матриц» со следующими возможностями:

### Базовая часть (обязательная)
- `create_matrix(rows, cols)` — создание матрицы, инициализированной нулями.
- `free_matrix(m)` — освобождение памяти (защита от double-free).
- `matrix_add(a, b)` — сложение двух матриц одинакового размера.
- `matrix_multiply(a, b)` — классическое матричное умножение (M×N × N×K).
- `matrix_from_array(data, rows, cols)` — создание матрицы из одномерного массива.
- `print_matrix(m)` — вывод матрицы в stdout (с сокращением для больших матриц).

### Индивидуальное задание (вариант 9)
- `matrix_multiply_elementwise(a, b)` — **поэлементное умножение**  
  `C[i][j] = A[i][j] * B[i][j]`, матрицы должны иметь одинаковый размер.

## 🛠 Обработка ошибок

Используется подход через **исключения C++**:
- `std::invalid_argument` — некорректные размеры, несовпадение размеров, `nullptr`.
- `std::bad_alloc` — не удалось выделить память.

Каждая функция проверяет входные параметры и не приводит к segfault/UB.

## Сборка и запуск

```bash
# Конфигурация
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Сборка
cmake --build build --parallel

# Запуск
./build/matrix_calculator
```

## Тесты

```bash
# Запуск всех тестов
cd build && ctest --verbose

# Проверка утечек памяти
valgrind --leak-check=full --error-exitcode=1 ./build/tests/matrix_tests
```

### Проверяемые сценарии
1. Создание матрицы (норма, 1×1, нулевые/отрицательные размеры).
2. Сложение матриц (норма, несовпадение размеров).
3. Матричное умножение (норма, вектор×вектор, несовпадение).
4. `matrix_from_array` (норма, `nullptr`).
5. **Вариант 9:** поэлементное умножение (норма, 1×1, нули, отрицательные, несовпадение).
6. Безопасное освобождение (включая double-free).

## Документация (Doxygen)

```bash
doxygen Doxyfile
# открыть docs/html/index.html в браузере
```

##  Docker

```bash
docker build -t matrix-calc .
docker run --rm matrix-calc
```

##  CI/CD (GitHub Actions)

При каждом `push`/`pull_request` автоматически:
1. Собирается проект.
2. Запускаются тесты (`ctest`).
3. Проверяются утечки памяти (`valgrind`).
4. Генерируется документация (`doxygen`).
5. HTML-документация выгружается как артефакт.

Файл конфигурации: `.github/workflows/ci.yml`.

## 📁 Структура проекта

```
matrix_calculator/
├── include/
│   └── matrix.h
├── src/
│   ├── matrix.cpp
│   └── main.cpp
├── tests/
│   ├── CMakeLists.txt
│   └── test_matrix.cpp
├── docs/
├── .github/workflows/
│   └── ci.yml
├── .clang-format
├── .gitignore
├── CMakeLists.txt
├── Dockerfile
├── Doxyfile
└── README.md
```

