/**
 * @file main.cpp
 * @brief Главный файл программы 
 */
#include <iostream>
#include <type_traits>
#include <cstdlib> // Для EXIT_SUCCESS

// Безопасный макрос SUB через generic-лямбду
// Убран пробел из [], заменены комментарии на /* */, обернуты аргументы
#define SUB(x, y) ([] (auto arg1, auto arg2) noexcept { \
    /* Compile-time защита: проверка типов через decltype */ \
    static_assert(std::is_arithmetic_v<decltype(arg1)> && std::is_arithmetic_v<decltype(arg2)>, \
        "Ошибка сборки: Макрос SUB принимает только числовые типы!"); \
    return arg1 - arg2; \
}((x), (y))) // Аргументы макроса внутри вызова тоже лучше брать в скобки





// Альтернатива макросу — шаблонная constexpr функция с концептами
template <typename T1, typename T2>
requires std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2>
constexpr auto SUB_SAFE(T1 a, T2 b) {
    return a - b;
}

int main(int argc, char** argv)
{
    // На Linux для корректного вывода кириллицы в терминале
    std::setlocale(LC_ALL, "");

    int a = 6;
    int b = 5;
    int c = 2;

    std::cout << "\n\n=================SUB" << std::endl;         
    std::cout << SUB(a, b) << std::endl;         // Вывод: 1
    std::cout << SUB(a, b) * c << std::endl;     // Вывод: 2
    std::cout << SUB(a, b + c) * c << std::endl; // Вывод: -2

    // Защита от Side-эффектов: 'a' увеличится строго ОДИН раз
    std::cout << "SUB с инкрементом: " << SUB(++a, b) << " (a теперь = " << a << ")" << std::endl;
    

    a = 6;
    b = 5;
    c = 2;
    std::cout << "\n\n=================SUB_SAFE" << std::endl;         
    std::cout << SUB_SAFE(a, b) << std::endl;         // Вывод: 1
    std::cout << SUB_SAFE(a, b) * c << std::endl;     // Вывод: 2
    std::cout << SUB_SAFE(a, b + c) * c << std::endl; // Вывод: -2

    return EXIT_SUCCESS;
}