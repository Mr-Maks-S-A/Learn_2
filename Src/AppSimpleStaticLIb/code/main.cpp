/**
 * @file main.cpp
 * @brief Главный файл программы 
 */

#include <iostream>
#include <string>
#include <Leaver.hpp> // Подключаем заголовочный файл библиотеки

int main() {
    // Настройка локализации для корректного вывода кириллицы в консоли
std::setlocale(LC_ALL, "Russian");

    std::cout << "Введите имя: ";
    std::string name;
    std::cin >> name;

    Leaver leaver;
    std::cout << leaver.leave(name) << std::endl;

    return EXIT_SUCCESS;
};