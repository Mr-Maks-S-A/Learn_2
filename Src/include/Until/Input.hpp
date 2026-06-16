#pragma once
#include <iostream>
#include <string>
#include <limits>
#include <type_traits>
#include <concepts>
#include <initializer_list>
#include <cctype>
#include <algorithm>

// 2. Объявление концепта
template <typename T>
concept ArithmeticNumber = 
    (std::integral<T> || std::floating_point<T>) 
    && !std::same_as<T, bool> 
    && !std::same_as<T, char>
    && !std::same_as<T, signed char>
    && !std::same_as<T, unsigned char>
    && !std::same_as<T, wchar_t>
    && !std::same_as<T, char8_t>
    && !std::same_as<T, char16_t>
    && !std::same_as<T, char32_t>;

/**
 * @brief Универсальная функция безопасного ввода числа с консоли в заданном диапазоне c списком исключений.
 * @tparam T Тип вводимого значения (должен быть арифметическим).
 * @param prompt Сообщение для пользователя.
 * @param min_val Минимально допустимое значение.
 * @param max_val Максимально допустимое значение.
 * @param exclude_list Список точечных ограничений которые не включаются
 * @return Считанное и проверенное значение.
 */
template <ArithmeticNumber T>
T get_input(const std::string& prompt,
            T min_val = std::numeric_limits<T>::lowest(),
            T max_val = std::numeric_limits<T>::max(),
            std::initializer_list<T> exclude_list = {});



/**
 * @brief Универсальная функция безопасного ввода данных с консоли с строгим соответствием на наличие нужного во включаемом списке.
 * @tparam T Тип вводимого значения.
 * @param prompt Сообщение для пользователя.
 * @param include_list Список точечных ограничений только которые и разрешены для ввода
 * @return Считанное и проверенное значение.
 */
template <typename T>
T get_input(        const std::string& prompt
             ,std::initializer_list<T> include_list);


/**
 * @brief Специализация функции безопасного ввода под строку с консоли в заданном диапазоне.
 * @param prompt Сообщение для пользователя.
 * @param min_val Минимально допустимое колличество символов.
 * @param max_val Максимально допустимое колличество символов.
 * @return Считанное и проверенное значение.
 */
template <typename T>
requires std::is_same_v<T, std::string>
std::string get_input(const std::string& prompt
                                 ,size_t min_len = 0
                                 ,size_t max_len = std::numeric_limits<size_t>::max());


/**
 * @brief Функция безопасного ввода строки с кастомной валидацией через предикат.
 * @param prompt Сообщение для пользователя.
 * @param validator Лямбда или функция, возвращающая true, если строка корректна.
 * @param error_msg Сообщение об ошибке, если валидация не прошла.
 */
template <typename T>
requires std::is_same_v<T, std::string>
std::string get_input(const std::string& prompt,
                      auto validator, // Упрощаем: пусть компилятор сам выведет тип лямбды
                      const std::string& error_msg = "Ошибка: некорректный формат строки.\n")
    requires std::predicate<decltype(validator), const std::string&>; // Накладываем ограничение, что это предикат
//реализация здесь (файл просто для отделения реализации из заголовочного)
#include <Until/Input.inl>