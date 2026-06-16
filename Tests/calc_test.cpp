/**
 * @file tests.cpp
 * @brief Модульные тесты для шаблонного калькулятора с использованием assert.
 */

#include <iostream>
#include <cassert>
#include <stdexcept>
#include <cmath>
#include <Calc/Calculator.hpp>

// Вспомогательная функция для сравнения чисел с плавающей точкой.
// Напрямую сравнивать double через == нельзя из-за погрешностей округления.
bool is_close(double a, double b, double epsilon = 1e-9) {
    return std::abs(a - b) < epsilon;
}

/**
 * @brief Тестирование калькулятора с типом double (классический режим).
 */
void test_double_calculator() {
    Calculator<double> calc;

    // Базовые операции
    assert(is_close(calc.evaluate("2 + 2"), 4.0));
    assert(is_close(calc.evaluate("5 - 3"), 2.0));
    assert(is_close(calc.evaluate("3 * 4"), 12.0));
    assert(is_close(calc.evaluate("10 / 4"), 2.5));

    // Приоритет операторов и пробелы
    assert(is_close(calc.evaluate("2 + 3 * 4"), 14.0));
    assert(is_close(calc.evaluate("  2  * 3  +  4  "), 10.0));

    // Скобки
    assert(is_close(calc.evaluate("(2 + 3) * 4"), 20.0));
    assert(is_close(calc.evaluate("10 / (5 - 3)"), 5.0));
    assert(is_close(calc.evaluate("((2 + 2) * 2) - 1"), 7.0));

    // Отрицательные числа и дробная часть
    assert(is_close(calc.evaluate("-5 + 3"), -2.0));
    assert(is_close(calc.evaluate("2.5 + 1.5"), 4.0));

    std::cout << "[ОК] Тесты для Calculator<double> успешно пройдены.\n";
}

/**
 * @brief Тестирование калькулятора с типом int (целочисленный режим).
 */
void test_int_calculator() {
    Calculator<int> calc;

    // В целочисленном режиме 5 / 2 должно быть равно 2 (остаток отбрасывается)
    assert(calc.evaluate("5 / 2") == 2);
    assert(calc.evaluate("2 + 3 * 3") == 11);
    assert(calc.evaluate("(10 - 1) / 3") == 3);

    std::cout << "[ОК] Тесты для Calculator<int> успешно пройдены.\n";
}

/**
 * @brief Тестирование обработки ошибок и исключений.
 */
void test_exceptions() {
    Calculator<double> calc;

    // 1. Тест на деление на ноль
    try {
        calc.evaluate("5 / 0");
        assert(false); // Если строка выше НЕ выбросила ошибку, этот assert завалит тест
    } catch (const std::runtime_error& e) {
        // Мы ожидаем эту ошибку, проверяем текст сообщения
        assert(std::string(e.what()) == "Деление на ноль");
    }

    // 2. Тест на забытую закрывающую скобку
    try {
        calc.evaluate("(2 + 2");
        assert(false);
    } catch (const std::runtime_error& e) {
        assert(std::string(e.what()) == "Ожидалась закрывающая скобка ')'");
    }

    // 3. Тест на мусор в конце строки
    try {
        calc.evaluate("2 + 2 )");
        assert(false);
    } catch (const std::runtime_error& e) {
        assert(std::string(e.what()) == "Обнаружены лишние символы в конце выражения");
    }

    // 4. Тест на пустую или некорректную строку
    try {
        calc.evaluate("abc");
        assert(false);
    } catch (const std::runtime_error& e) {
         assert(std::string(e.what()) == "Ошибка чтения числа");
    }

    std::cout << "[ОК] Тесты обработки исключений успешно пройдены.\n";
}

int main() {
    std::setlocale(LC_ALL, "Russian");
    
    std::cout << "Запуск модульных тестов...\n";
    std::cout << "------------------------------------\n";

    test_double_calculator();
    test_int_calculator();
    test_exceptions();

    std::cout << "------------------------------------\n";
    std::cout << "ВСЕ ТЕСТЫ ВЫПОЛНЕНЫ УСПЕШНО!\n";
    
    return 0;
}