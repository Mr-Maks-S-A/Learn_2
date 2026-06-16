/**
 * @file main.cpp
 * @brief Главный файл программы-калькулятора.
 * * Использует утилиты безопасного ввода и шаблонный парсер выражений
 * для выполнения математических операций, выбранных пользователем.
 */
#include <iostream> 
#include <cstdint>
#include <locale> 
#include <string>
#include <limits>
#include <stdexcept>
#include <Calc/Calculator.hpp>
#include <Until/Input.hpp>




int main() {
    std::setlocale(LC_ALL, "Russian");
    Calculator<double> calc;

    std::cout << "=== Программа: Строковый рекурсивный калькулятор ===\n";

    // Объявляем строго ДО цикла while
    const std::string allowed_chars = "0123456789+-*/()^. ";

    while (true) {
        std::string expr_input = get_input<std::string>(
            "Введите выражение [или для выхода введите 'q' или 'Q']: ",
            [&allowed_chars](const std::string& str) {
                return str.find_first_not_of(allowed_chars) == std::string::npos;
            },
            "Ошибка: выражение содержит запрещенные символы! Разрешены только цифры, пробелы и (+ - * / . ( ) ^ ) [Для выхода введите 'q' или 'Q']\n"
        );

        if (expr_input == "q" || expr_input == "Q") {
            std::cout << "\nЗавершение работы программы. До свидания!\n";
            break;
        }

        try {
            double result = calc.evaluate(expr_input);
            std::cout << "Результат: " << result << "\n";
        } 
        catch (const std::runtime_error& e) {
            std::cerr << "Ошибка вычисления: " << e.what() << "\n";
        }

        std::cout << "\n-----------------------------------------\n\n";
    }

        return EXIT_SUCCESS;

}
