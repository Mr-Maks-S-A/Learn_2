#include <iostream>
#include <string>

int main() {
    // Настройка локализации для корректного ввода/вывода кириллицы в консоли
    std::setlocale(LC_ALL, "Russian");

    std::cout << "Введите имя: ";
    std::string name;
    std::getline(std::cin, name);

    std::cout << ("Здравствуйте, " + name + "!") << std::endl;

    return EXIT_SUCCESS;
}