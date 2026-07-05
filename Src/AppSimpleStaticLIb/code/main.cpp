/**
 * @file main.cpp
 * @brief Главный файл программы 
 */

#include <iostream>
#include <string>
#include <Greeter.hpp> // Подключаем заголовочный файл библиотеки

int main() {
    // Настройка локализации для корректного вывода кириллицы в консоли
    std::setlocale(LC_ALL, "Russian");
    
    std::cout << "Введите имя: ";
    std::string name;
    std::getline(std::cin, name); // Используем getline, чтобы корректно считывать имена с пробелами

    Greeter greeter;
    std::string greeting = greeter.greet(name);

    std::cout << greeting << std::endl;

    return EXIT_SUCCESS;
};