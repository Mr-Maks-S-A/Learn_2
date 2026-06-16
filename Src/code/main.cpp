/**
 * @file main.cpp
 * @brief Главный файл программы 
 */

#include <iostream>

// 1. Объявляем символьную константу MODE.
// Меняй это значение (0, 1 или любое другое), чтобы протестировать разные режимы.
#define MODE 1

// 2. Проверяем, что константа определена. Если нет — ошибка компиляции.
#ifndef MODE
#error MODE is not defined! Please define MODE to compile the program.
#endif

// 3. Условная компиляция функции add (только для боевого режима)
#if MODE == 1
int add(int a, int b) {
    return a + b;
}
#endif

int main() {
    // Настройка локализации, чтобы корректно отображался русский язык в консоли
    setlocale(LC_ALL, "Russian");

    // 4. Проверка значения MODE во время компиляции для логики программы
#if MODE == 0
    std::cout << "Работаю в режиме тренировки" << std::endl;

#elif MODE == 1
    std::cout << "Работаю в боевом режиме" << std::endl;
    
    int num1 = 0;
    int num2 = 0;
    
    std::cout << "Введите число 1: ";
    std::cin >> num1;
    std::cout << "Введите число 2: ";
    std::cin >> num2;
    
    std::cout << "Результат сложения: " << add(num1, num2) << std::endl;

#else
    std::cout << "Неизвестный режим. Завершение работы" << std::endl;

#endif

    return EXIT_SUCCESS;
};