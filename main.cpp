#include <cstdint>
#include <iostream> // Для потоков ввода/вывода
#include <clocale>  // Библиотека для работы с локалью
#include <cstdlib>  // Для макросов EXIT_SUCCESS

#include <string>
#include <type_traits> // Необходимо для is_copy_assignable_v/is_copy_constructible_v
#include <concepts> // Для std::derived_from, std::integral и т.д.
#include <limits> // Обязательно для std::numeric_limits
#include <cassert>     // Обязательно для работы assert()

#include <iomanip> // Для std::fixed и std::setprecision


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

        while (std::isspace(std::cin.peek()) && std::cin.peek() != '\n') {
            std::cin.get();
        }

        if constexpr (std::is_unsigned_v<T>) {
            if (std::cin.peek() == '-') {
                std::cout << "Ошибка: введено отрицательное число для беззнакового поля.\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
        }

        // 1. Проверка на корректность типа данных
        if (!(std::cin >> value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: введите корректное число.\n";
            continue;
        }

        while (std::isspace(std::cin.peek()) && std::cin.peek() != '\n') {
            std::cin.get();
        }

        if (std::cin.peek() != '\n' && std::cin.peek() != EOF) {
            std::cout << "Ошибка: обнаружены лишние символы после числа.\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::cin.ignore();

        // 2. Проверка попадания в диапазон [min_val, max_val]
        if (value < min_val || value > max_val) {
            std::cout << "Ошибка: значение должно быть в диапазоне от "
            << min_val << " до " << max_val << ".\n";
            // Буфер уже чист благодаря шагу 4, очищать заново не нужно
            continue;
        }

        return value;
    }
};

// 2. Полная специализация шаблона для std::string
template <typename T>
requires std::is_same_v<T, std::string>
std::string get_input(const std::string& prompt,
                      size_t min_len = 0,
                      size_t max_len = std::numeric_limits<size_t>::max())
{
    std::string value;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin >> std::ws, value)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка ввода.\n";
            continue;
        }

        if (value.length() < min_len || value.length() > max_len) {
            std::cout << "Ошибка: длина строки должна быть от " << min_len << " до " << max_len << ".\n";
            continue;
        }
        return value;
    }
}


// Определение структуры для хранения информации о банковском счёте
struct Address {
    uint64_t Index;              //индекс
    uint64_t HomeNumber;         //номер дома
    uint64_t ApartmentNumber;    //номер квартиры
    std::string City;            //Город
    std::string Street;          //улица
};


/**
 * @brief Выводит информацию структуре счёте в консоль.
 * @param account Константная ссылка на структуру счёта.
 */
void print_address(const Address& account) {
    std::cout << "\n========================================\n"
    << "Город          : " << account.City <<std::endl
    << "Улица          : " << account.Street <<std::endl
    << "Номер дома     : " << account.HomeNumber <<std::endl
    << "Номер квартиры : " << account.ApartmentNumber <<std::endl
    << "Индекс         : " << account.Index <<std::endl
    << "========================================\n\n";
};


int main() {
    // Устанавливаем локаль для корректного вывода кириллицы в консоль
    std::setlocale(LC_ALL, "Russian");

    std::cout << "=== Настройка первого адреса ===\n";
    Address firstAddress;
    firstAddress.City            = get_input<std::string>("Введите город: ", 1, 100);
    firstAddress.Street          = get_input<std::string>("Введите улицу: ", 1, 100);
    firstAddress.HomeNumber      = get_input<uint64_t>("Введите номер дома: ", 1);
    firstAddress.ApartmentNumber = get_input<uint64_t>("Введите номер квартиры: ", 1);
    firstAddress.Index           = get_input<uint64_t>("Введите индекс (до 6 знаков): ", 1, 999999);

    std::cout << "\nРезультат вывода:\n";
    print_address(firstAddress);


    std::cout << "=== Демонстрация готовых экземпляров (Пример работы) ===\n";
    Address moscowAddress = {
        123456,
        12,
        8,
        "Москва",
        "Арбат"
    };

    Address izhevskAddress = {
        953769,
        59,
        143,
        "Ижевск",
        "Пушкина"
    };

    std::cout << "\nРезультат вывода:\n";
    print_address(moscowAddress);
    print_address(izhevskAddress);

    return EXIT_SUCCESS;
}
