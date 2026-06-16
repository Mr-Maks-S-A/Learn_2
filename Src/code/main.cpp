/**
 * @file main.cpp
 * @brief Главный файл программы демонстрации работы счётчика.
 * Использует утилиты безопасного ввода и инкапсулированный класс Counter
 * для работы с командами пользователя через консоль.
 */

#include <iostream> 
#include <cstdint>
#include <locale> 
#include <string>
#include <limits>
#include <stdexcept>

#include <Counter/Counter.hpp>
#include <Until/Input.hpp>


void  worc(Counter &counter) {
    while (true) {
        // Используем перегрузку с include_list для строгого выбора команд
        char command = get_input<char>(
            "Введите команду ('+', '-', '=' или 'q'): ",
            {'+', '-', '=', 'q', 'Q'}
        );

        if (command == 'q' || command == 'Q') {
            std::cout << "До свидания!\n";
            break;
        }

        switch (command) {
            case '+':
                counter.increment();
                break;
            case '-':
                counter.decrement();
                break;
            case '=':
                std::cout << counter.get_value() << "\n";
                break;
            default:
                // Сюда код никогда не зайдёт, так как get_input отфильтрует всё остальное
                break;
        }
    }
}

int main() {
    std::setlocale(LC_ALL, "Russian");

    while (true) {
        // Шаг 1. Строго проверяем ответ пользователя (да/нет) через include_list
        // Передаем std::string, чтобы работали многосимвольные ответы
        std::string answer = get_input<std::string>(
            "Вы хотите указать начальное значение счётчика? Введите да(y/yes) или нет(n/no): ",
            {"да", "y", "Y", "yes", "нет", "n", "N", "no"}
        );

        // Шаг 2. Инициализация счётчика в зависимости от ответа
        if (answer == "да" || answer == "y" || answer == "Y" || answer == "yes") {
            // Используем первую перегрузку для безопасного ввода целого числа (int)
            int initial_val = get_input<int>("Введите начальное значение счётчика: ");
            
            Counter counter(initial_val);
             worc(counter);
        } else {
            Counter counter;
             worc(counter);
        }

        // Шаг 3. Спрашиваем про выход или повтор
        std::string choice = get_input<std::string>(
            "=== Введите 'q' для выхода или 'c'/'continue' для повтора ===\n",
            {"q", "Q", "c", "C", "continue"}
        );

        if (choice == "q" || choice == "Q") {
            break; 
        }
    }

    return EXIT_SUCCESS;
}