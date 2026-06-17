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
#include <Calc/SimpleMath.hpp>
#include <Until/Input.hpp>




void runPrimitiveMode();
void runAdvancedMode(Calculator<double>& calc);

int main() {
    std::setlocale(LC_ALL, "Russian");
    Calculator<double> calc;

    std::cout << "=== Программа: Многорежимный Калькулятор ===\n";

    while (true) {
        std::cout << "Выберите режим работы:\n";
        std::cout << "1. Примитивный режим (свободные функции hpp/cpp)\n";
        std::cout << "2. Полноценный режим (строковый парсер выражений)\n";
        std::cout << "3. Выход из программы\n";

        // Используем твою перегрузку get_input со списком разрешенных значений
        std::string choice = get_input<std::string>("Ваш выбор (1-3): ", {"1", "2", "3"});

        if (choice == "3") {
            std::cout << "\nЗавершение работы программы. До свидания!\n";
            break;
        }

        std::cout << "\n-----------------------------------------\n";
        if (choice == "1") {
            runPrimitiveMode();
        } else if (choice == "2") {
            runAdvancedMode(calc);
        }
        std::cout << "-----------------------------------------\n\n";
    }

    return EXIT_SUCCESS;
}


void runPrimitiveMode() {
    std::cout << ">>> Запущен ПРИМИТИВНЫЙ режим (hpp/cpp свободные функции) <<<\n";
    
    while (true) {
        double num1 = get_input<double>("Введите первое число (или '0' и затем 'q' на следующем шаге для выхода): ");
        
        std::string op = get_input<std::string>(
            "Выберите операцию (+, -, *, /, ^) [или 'q' для возврата в меню]: ", 
            {"+", "-", "*", "/", "^", "q", "Q"}
        );
        
        if (op == "q" || op == "Q") {
            break;
        }

        double num2 = get_input<double>("Введите второе число: ");

        try {
            double result = 0.0;
            if (op == "+") result = add(num1, num2);
            else if (op == "-") result = subtract(num1, num2);
            else if (op == "*") result = multiply(num1, num2);
            else if (op == "/") result = divide(num1, num2);
            else if (op == "^") result = power(num1, num2);

            std::cout << "Результат: " << result << "\n";
        } 
        catch (const std::runtime_error& e) {
            std::cerr << "Ошибка вычисления: " << e.what() << "\n";
        }
        std::cout << "\n";
    }
}


void runAdvancedMode(Calculator<double>& calc) {
    std::cout << ">>> Запущен ПОЛНОЦЕННЫЙ режим (строковый рекурсивный парсер) <<<\n";
    const std::string allowed_chars = "0123456789+-*/()^. ";

    while (true) {
        std::string expr_input = get_input<std::string>(
            "Введите выражение [или 'q' для возврата в меню]: ",
            [&allowed_chars](const std::string& str) {
                return str.find_first_not_of(allowed_chars) == std::string::npos;
            },
            "Ошибка: выражение содержит запрещенные символы! Разрешены только цифры, пробелы и (+ - * / . ( ) ^ )\n"
        );

        if (expr_input == "q" || expr_input == "Q") {
            break;
        }

        try {
            double result = calc.evaluate(expr_input);
            std::cout << "Результат: " << result << "\n";
        } 
        catch (const std::runtime_error& e) {
            std::cerr << "Ошибка вычисления: " << e.what() << "\n";
        }
        std::cout << "\n";
    }
}