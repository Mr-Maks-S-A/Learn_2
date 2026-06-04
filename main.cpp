#include <cstdint>
#include <iostream> 
#include <locale> 
#include <cstdlib>  
#include <vector>
#include <string_view>
#include <string>
#include <cctype>
#include <cwctype>
#include <unordered_map>
#include <fstream>
#include <algorithm> // Добавлено для std::sort
#include <limits>

#include <MySmartType.hpp> 

class Addresse {
private:
    uint32_t m_house = 0;
    uint32_t m_apartment = 0;
    
    CacheString m_city;
    CacheString m_street;

public:
    Addresse(std::string new_city,
             std::string new_street,
             uint32_t new_house,
             uint32_t new_apartment) 
        : m_house(new_house),
          m_apartment(new_apartment),
          m_city(std::move(new_city)),
          m_street(std::move(new_street)) {}

    ~Addresse() = default;
        
    // Геттер для получения имени города (возвращает std::string_view из кэша без аллокаций)
    [[nodiscard]] std::string_view get_city() const {
        return static_cast<const std::string&>(m_city);
    }

    [[nodiscard]] std::string get_output_address() const {
        using namespace std::string_literals;
        return ""s + m_city + ", " + m_street + ", " + 
               std::to_string(m_house) + ", " + std::to_string(m_apartment);
    }
};

int main() {
    std::setlocale(LC_ALL, "ru_RU.UTF-8");

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
    
    in_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<Addresse> addresses;
    addresses.reserve(count); 

    for (int i = 0; i < count; ++i) {
        std::string city;
        std::string street;
        uint32_t house = 0;
        uint32_t apartment = 0;

        if (std::getline(in_file, city) &&
            std::getline(in_file, street) &&
            (in_file >> house) &&
            (in_file >> apartment)) {
            
            in_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            addresses.emplace_back(std::move(city), std::move(street), house, apartment);
        } else {
            std::cerr << "Предупреждение: Файл поврежден или закончился раньше времени на блоке " << i + 1 << std::endl;
            break;
        }
    }
    in_file.close(); 

    // --- СОРТИРОВКА ПО АЛФАВИТУ ---
    // Используем std::sort с лямбда-компаратором. Благодаря std::string_view
    // сравнение строк происходит без копирования данных из кэша.
    std::sort(addresses.begin(), addresses.end(), [](const Addresse& lhs, const Addresse& rhs) {
        return lhs.get_city() < rhs.get_city();
    });

    std::ofstream out_file("Sorce/out.txt");
    if (!out_file.is_open()) {
        std::cerr << "Не удалось открыть файл out.txt для записи!" << std::endl;
        return EXIT_FAILURE;
    }

    out_file << addresses.size() << "\n";

    // Выводим адреса по порядку (после сортировки они уже идут от А до Я)
    for (const auto& addr : addresses) {
        out_file << addr.get_output_address() << "\n";
    }
    out_file.close(); 

    std::cout << "Обработка и сортировка завершены успешно. Результат сохранен в out.txt" << std::endl;
    return EXIT_SUCCESS;
}