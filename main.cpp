#include <cstdint>
#include <iostream> // Для потоков ввода/вывода
#include <clocale>  // Библиотека для работы с локалью
#include <cstdlib>  // Для макросов EXIT_SUCCESS

#include <string>

class Counter {
private:
    int m_value; // Приватное поле, закрытое от внешнего мира

public:
    // Конструктор по умолчанию (начальное значение 1)
    Counter() : m_value(1) {
        std::cout << "Вызван счётчик по умолчанию >> значение равно = "<< m_value<<std::endl;
    };

    // Конструктор с заданным начальным значением
    Counter(int initial_value) : m_value(initial_value) {
        std::cout << "Вызван счётчик c параметром >> значение равно = "<< m_value<<std::endl;
    }

    ~Counter() = default;

    // Метод увеличения на 1
    void increment() {
        m_value++;
    }

    // Метод уменьшения на 1
    void decrement() {
        m_value--;
    }

    // Метод просмотра текущего значения (const, так как не меняет состояние объекта)
    int get_value() const {
        return m_value;
    }
};


void test(Counter &counter){

    char command;
    while (true) {
        std::cout << "Введите команду ('+', '-', '=' или 'q'): ";
        std::cin >> command;

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
                std::cout << "Неизвестная команда!\n";
                break;
        }
    }

}


int main() {
    // Устанавливаем локаль для корректного вывода кириллицы в консоль
    std::setlocale(LC_ALL, "Russian");


    do{
        std::string answer;
        std::cout << "Вы хотите указать начальное значение счётчика? Введите да(y/yes/lf) или нет(n/no): ";
        std::cin >> answer;

        // Вспомогательная лямбда или тернарный оператор для получения начального значения
        int initial_val = 1;
        if (   answer == "да"
            || answer == "y"
            || answer == "Y"
            || answer == "yes" 
            || answer == "if") {
            std::cout << "Введите начальное значение счётчика: ";
            std::cin >> initial_val;
            Counter counter(initial_val);
            test(counter);
        }else{
            Counter counter;
            test(counter);
        }

        // Перед чтением строки/символа ОЧИЩАЕМ буфер от остаточных '\n' из функции test()
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "=== Введите 'q' для выхода или нажмите [Enter], чтобы повторить ===\n";
        
        // Читаем всю строку (если нажат просто Enter, строка будет пустой)
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "q" || choice == "Q") {
            break; // Выходим из do-while
        }
    }while (true);

    return EXIT_SUCCESS;
}
