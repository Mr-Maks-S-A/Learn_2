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
#include <algorithm> // Обязательно для std::ranges::any_of


//Открыл для себя концепты
template <typename T>
concept ArithmeticNumber = 
    (std::integral<T> || std::floating_point<T>) 
    && !std::same_as<T, bool> 
    && !std::same_as<T, char>
    && !std::same_as<T, wchar_t>;



/**
 * @brief Универсальная функция безопасного ввода числа с консоли в заданном диапазоне.
 * @tparam T Тип вводимого значения (должен быть арифметическим).
 * @param prompt Сообщение для пользователя.
 * @param min_val Минимально допустимое значение.
 * @param max_val Максимально допустимое значение.
 * @param exclude_list Список точечных ограничений
 * @return Считанное и проверенное значение.
 */
template <ArithmeticNumber T>
T get_input(const std::string& prompt,
            T min_val = std::numeric_limits<T>::lowest(),
            T max_val = std::numeric_limits<T>::max(),
            std::initializer_list<T> exclude_list = {}) {
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

        // 3. НОВОЕ: Проверка точечных ограничений (исключений)
        if (std::ranges::any_of(exclude_list, [value](T excluded) { return value == excluded; })) {
            std::cout << "Ошибка: это конкретное значение (" << value << ") запрещено для ввода.\n";
            continue;
        }

        return value;
    }
};




// Проверка: гарантируем, что калькулятор не будет создан для приведённых ниже типов
// requires std::is_arithmetic_v<T> // int, double, float (включая signed/unsigned, long, short, char, wchar_t) и bool
// static_assert(!std::is_same_v<T, bool>, "Калькулятор не поддерживает тип bool!");
// static_assert(!std::is_same_v<T, char> &&
//               !std::is_same_v<T, wchar_t>, "Калькулятор не поддерживает символьные типы!");

// альтернатива ограничения типов до int, double, float (включая signed/unsigned, long, short)
// requires (std::integral<T> || std::floating_point<T>) 
//         && !std::same_as<T, bool> 
//         && !std::same_as<T, char>
template <ArithmeticNumber T>
class Calculator{
    public:
        Calculator() {
            #ifdef ENABLE_LOGGING
            std::clog << "[LOG]: Вызван конструктор по умолчанию Calculator\n"
                    << "num1 = "<< __num1 <<std::endl
                    << "num2 = "<< __num2 <<std::endl;
            #endif
        };
        Calculator(T new_num1 , T new_num2){
            if(set_num1(new_num1))
            std::cerr << "Ошибка: переменные не могут равняться 0!"<< std::endl;
            
            if(set_num2(new_num2))
            std::cerr << "Ошибка: переменные не могут равняться 0!"<< std::endl;
           


            #ifdef ENABLE_LOGGING
            std::clog << "[LOG]: Вызван конструктор c параметрами new_num1 new_num2 Calculator\n"
                    << "num1 = "<< __num1 <<std::endl
                    << "num2 = "<< __num2 <<std::endl;
            #endif
        };
        ~Calculator() = default;

    private:
        T __num1 = 1, __num2 = 1 ; 
    public:
        bool set_num1(T new_num1) { 
            if (new_num1 == 0) return false;
            this->__num1 = new_num1; 
            return true; 
        };
        
        bool set_num2(T new_num2) { 
            if (new_num2 == 0) return false;
            this->__num2 = new_num2; 
            return true; 
        };

        T get_num1() const {return this->__num1;};
        T get_num2() const {return this->__num2;};
    public:
        T add()          { return (__num1 + __num2); };
        T multiply()     { return (__num1 * __num2); };
        T subtract_1_2() { return (__num1 - __num2); };
        T subtract_2_1() { return (__num2 - __num1); };
        
        T divide_1_2()   { 
            if (__num2 == 0) {
                std::cerr << "Ошибка: Деление на ноль (m_num2 == 0)!"<< std::endl;
                return static_cast<T>(0);
            };
         
            return (__num1 / __num2);
         };
        
        T divide_2_1()   { 
            if (__num1 == 0) {
                std::cerr << "Ошибка: Деление на ноль (m_num1 == 0)!"<< std::endl;
                return static_cast<T>(0);
            }

            return (__num2 / __num1); 
        };
};


int main() {
    // Устанавливаем локаль для корректного вывода кириллицы в консоль
    std::setlocale(LC_ALL, "Russian");

#ifdef DEBUG_MODE
    std::cout << "[SYSTEM]: Запуск комплексных Unit-тестов...\n";
    
    // =========================================================================
    // 1. ТЕСТЫ РАЗРЕШЕННЫХ ТИПОВ (РАНТАЙМ С ИСПОЛЬЗОВАНИЕМ ASSERT)
    // =========================================================================

    // Тест 1.1: Стандартные целочисленные со знаком (int, short, long, long long)
    Calculator<short> short_calc(10, 2);
    assert(short_calc.add() == 12);
    assert(short_calc.divide_1_2() == 5);

    Calculator<long long> ll_calc(3000000000LL, 1000000000LL);
    assert(ll_calc.subtract_1_2() == 2000000000LL);
    assert(ll_calc.multiply() == 3000000000000000000LL);

    // Тест 1.2: Беззнаковые типы (unsigned int, unsigned short, uint64_t)
    // ВНИМАНИЕ: subtract_2_1() для (10, 5) вернет 5 - 10 = -5.
    // Для беззнакового типа это вызовет "underflow" (переполнение вниз), что легально,
    // но результат будет равен (Max_Value - 4). Проверим это корректно:
    Calculator<unsigned int> u_calc(10, 5);
    assert(u_calc.add() == 15);
    assert(u_calc.subtract_1_2() == 5);
    // Для унарного минуса/underflow в unsigned:
    assert(u_calc.subtract_2_1() == static_cast<unsigned int>(-5)); 

    // Тест 1.3: Типы с плавающей точкой (float, double, long double)
    Calculator<float> float_calc(7.5f, 2.5f);
    assert(float_calc.add() == 10.0f);
    assert(float_calc.divide_1_2() == 3.0f);

    Calculator<long double> ld_calc(0.5L, 0.25L);
    assert(ld_calc.multiply() == 0.125L);
    assert(ld_calc.divide_2_1() == 0.5L);

    // Тест 1.4: Проверка динамического изменения через сеттеры (double)
    Calculator<double> test_setter_calc;
    test_setter_calc.set_num1(4.0);
    test_setter_calc.set_num2(2.0);
    assert(test_setter_calc.get_num1() == 4.0);
    assert(test_setter_calc.get_num2() == 2.0);
    assert(test_setter_calc.divide_1_2() == 2.0);


    // =========================================================================
    // 2. ТЕСТЫ ЗАПРЕЩЕННЫХ ТИПОВ (КОМПИЛЯЦИЯ С ИСПОЛЬЗОВАНИЕМ STATIC_ASSERT)
    // после изменения на концепты компилятор просто не даст собрать с этими типами
    // ========================================================================= 
    // static_assert(!std::is_constructible_v<Calculator<bool>>, "Тест провален: bool не должен поддерживаться!");
    // static_assert(!std::is_constructible_v<Calculator<char>>, "Тест провален: char не должен поддерживаться!");
    // static_assert(!std::is_constructible_v<Calculator<wchar_t>>, "Тест провален: wchar_t не должен поддерживаться!");

    std::cout << "[SYSTEM]: Все Unit-тесты для разрешенных модификаторов успешно пройдены!\n\n";
#endif

    Calculator<double> calc;

    do {
        
        //get_input реализует ограничения на вводимые параметры (наименьшее разрещённое значение, максимальное разрещённое значение, и список точечных ограничений)

        calc.set_num1(get_input<double>("Введите num1: "
                                        ,std::numeric_limits<double>::lowest()
                                        ,std::numeric_limits<double>::max()
                                        ,{0.0})); 
        calc.set_num2(get_input<double>("Введите num2: "
                                        ,std::numeric_limits<double>::lowest()
                                        ,std::numeric_limits<double>::max()
                                        ,{0.0})); 


        // Форматируем вывод вещественных чисел (убирает артефакты округления)
        std::cout << std::fixed << std::setprecision(6);

        std::cout << "num1 + num2 = " << calc.add() << "\n";
        std::cout << "num1 - num2 = " << calc.subtract_1_2() << "\n";
        std::cout << "num2 - num1 = " << calc.subtract_2_1() << "\n";
        std::cout << "num1 * num2 = " << calc.multiply() << "\n";
        std::cout << "num1 / num2 = " << calc.divide_1_2() << "\n";
        std::cout << "num2 / num1 = " << calc.divide_2_1() << "\n\n\n";

        std::cout << "=== Введите 'q' для выхода или нажмите Enter, чтобы продолжить ===\n";
        if (std::cin.peek() == 'q' || std::cin.peek() == 'Q') {
            break; // Мгновенный выход, если ввели 'q'
        }
    } while (true);

    return EXIT_SUCCESS;
}
