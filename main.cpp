#include <cstdint>
#include <iostream> // Для потоков ввода/вывода
#include <locale> // Библиотека для работы с локалью
#include <cstdlib>  // Для макросов EXIT_SUCCESS

#include <vector>
#include <string_view>
#include <string>
#include <cctype>
#include <cwctype>
#include <unordered_map>

#include <fstream>

#include <MySmartType.hpp> //моя кастомная наработка для создания умных типов данных по политикам





class Addresse {
private:
    uint32_t m_house = 0;
    uint32_t m_apartment = 0;
    
    CacheString m_city;
    CacheString m_street;

public:
    // Конструктор исправлен: имена полей приведены в соответствие, добавлен std::move
    Addresse(std::string new_city
            ,std::string new_street
            ,uint32_t new_house
            ,uint32_t new_apartment): m_house(new_house)
                                     ,m_apartment(new_apartment)
                                     ,m_city(std::move(new_city))
                                     ,m_street(std::move(new_street)) {}

    ~Addresse() = default;
        
    [[nodiscard]] std::string get_output_address() const {
        using namespace std::string_literals;
        
        // Сборка строки работает автоматически за счет неявного вызова оператора приведения в SmartVar/CacheString
        return ""s + m_city + ", " + m_street + ", " + 
               std::to_string(m_house) + ", " + std::to_string(m_apartment);
    }
};


int main() {
    // Устанавливаем локаль для корректного вывода кириллицы в консоль
    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    // 1. Открываем файл для чтения
    std::ifstream in_file("Sorce/in.txt");
    if (!in_file.is_open()) {
        std::cerr << "Не удалось открыть файл in.txt для чтения!" << std::endl;
        return EXIT_FAILURE;
    }

    int count = 0;
    if (!(in_file >> count)) {
        std::cerr << "Ошибка чтения количества адресов!" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Сбрасываем оставшийся символ новой строки (\n) после чтения числа, 
    // чтобы последующий std::getline не прочитал пустую строку.
    in_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Контейнер для хранения объектов
    std::vector<Addresse> addresses;
    addresses.reserve(count); // Оптимизация выделения памяти

    // 2. Построчное чтение блоков адресов
    for (int i = 0; i < count; ++i) {
        std::string city;
        std::string street;
        uint32_t house = 0;
        uint32_t apartment = 0;

        // Используем std::getline, так как в названиях городов/улиц могут быть пробелы (напр. "Нижний Новгород")
        if (std::getline(in_file, city) &&
            std::getline(in_file, street) &&
            (in_file >> house) &&
            (in_file >> apartment)) {
            
            // Сбрасываем \n после чтения чисел перед следующим циклом getline
            in_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // Создаем объект. Тримминг и Capitalize отработают внутри конструктора через политики!
            addresses.emplace_back(city, street, house, apartment);
        } else {
            std::cerr << "Предупреждение: Файл поврежден или закончился раньше времени на блоке " << i + 1 << std::endl;
            break;
        }
    }
    in_file.close(); // Закрываем входной файл

    // 3. Открываем файл для записи
    std::ofstream out_file("Sorce/out.txt");
    if (!out_file.is_open()) {
        std::cerr << "Не удалось открыть файл out.txt для записи!" << std::endl;
        return EXIT_FAILURE;
    }

    // Записываем итоговое количество успешно прочитанных адресов
    out_file << addresses.size() << "\n";

    // Выводим адреса в обратном порядке при помощи обратных итераторов (reverse iterators)
    for (auto it = addresses.rbegin(); it != addresses.rend(); ++it) {
        out_file << it->get_output_address() << "\n";
    }
    out_file.close(); // Закрываем выходной файл

    std::cout << "Обработка завершена успешно. Результат сохранен в out.txt" << std::endl;
    return EXIT_SUCCESS;

    return EXIT_SUCCESS;
}
