#include <iostream> // Для потоков ввода/вывода
#include <clocale>  // Библиотека для работы с локалью
#include <cstdlib>  // Для макросов EXIT_SUCCESS

#include <string>
#include <type_traits> // Необходимо для is_copy_assignable_v/is_copy_constructible_v
#include <concepts> // Для std::derived_from, std::integral и т.д.
#include <limits> // Обязательно для std::numeric_limits
#include <cassert>     // Обязательно для работы assert()

/**
 * @brief Универсальная функция безопасного ввода числа с консоли в заданном диапазоне.
 * @tparam T Тип вводимого значения (должен быть арифметическим).
 * @param prompt Сообщение для пользователя.
 * @param min_val Минимально допустимое значение.
 * @param max_val Максимально допустимое значение.
 * @return Считанное и проверенное значение.
 */
template <typename T>
requires std::is_arithmetic_v<T>
T get_input(const std::string& prompt,
            T min_val = std::numeric_limits<T>::lowest(),
            T max_val = std::numeric_limits<T>::max()) {
    T value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;

        // 1. Проверка на корректность типа данных
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите корректное число.\n";
            continue;
        }

        // 2. Проверка попадания в диапазон [min_val, max_val]
        if (value < min_val || value > max_val) {
            std::cout << "Ошибка: значение должно быть в диапазоне от "
            << min_val << " до " << max_val << ".\n";

            // Очищаем буфер, так как ввод некорректен, и идем на следующую итерацию
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // Очищаем буфер от лишних символов (например, если ввели "10 abc")
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
};



enum class Month {
     Exit       = 0     //выход
    ,January    = 1     //Январь
    ,February   = 2     //Февраль
    ,March      = 3     //Март
    ,April      = 4     //Апрель
    ,May        = 5     //Май
    ,June       = 6     //Июнь
    ,July       = 7     //Июль
    ,August     = 8     //Август
    ,September  = 9     //Сентябрь
    ,October    = 10    //Октябрь
    ,November   = 11    //Ноябрь
    ,December   = 12    //Декабрь
    ,Final      = 13    //Указатель на окончание
 };


/**
 * @brief Обрабатывает числовой ввод, преобразует в месяц и выводит результат.
 * @param input_code Число, введенное пользователем.
 * @return true, если программа должна продолжить работу; false, если выбран выход.
 */
bool process_month_selection(int input_code) {
    // Проверяем рантайм-значение int перед кастом.
    // Оно должно быть в диапазоне [Month::Exit, Month::Final)
    assert(input_code >= static_cast<int>(Month::Exit) &&
    input_code < static_cast<int>(Month::Final) &&
    "input_code выходит за граници заранее определённых значений");

    Month month = static_cast<Month>(input_code);

    switch (month) {
        case Month::Exit:
            std::cout << "До свидания" << std::endl;
            return false; // Сигнал к завершению цикла
        case Month::January:   std::cout << "Январь" << std::endl; break;
        case Month::February:  std::cout << "Февраль" << std::endl; break;
        case Month::March:     std::cout << "Март" << std::endl; break;
        case Month::April:     std::cout << "Апрель" << std::endl; break;
        case Month::May:       std::cout << "Май" << std::endl; break;
        case Month::June:      std::cout << "Июнь" << std::endl; break;
        case Month::July:      std::cout << "Июль" << std::endl; break;
        case Month::August:    std::cout << "Август" << std::endl; break;
        case Month::September: std::cout << "Сентябрь" << std::endl; break;
        case Month::October:   std::cout << "Октябрь" << std::endl; break;
        case Month::November:  std::cout << "Ноябрь" << std::endl; break;
        case Month::December:  std::cout << "Декабрь" << std::endl; break;
    }
    return true; // Продолжаем работу
}


int main() {
    // Устанавливаем локаль для корректного вывода кириллицы в консоль
    std::setlocale(LC_ALL, "Russian");

    while (true) {
        // Динамически вычисляем границы на основе enum, исключая маркер Final
        int input = get_input<int>(
            "Введите номер месяца: ",
            static_cast<int>(Month::Exit),
            static_cast<int>(Month::Final) - 1
        );

        // Передаем валидное число на обработку. Если вернулся false — выходим из цикла
        if (!process_month_selection(input)) {
            break;
        }
    }

    return EXIT_SUCCESS;
}
