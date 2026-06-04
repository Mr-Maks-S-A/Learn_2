#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include <chrono>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <limits>
#include <clocale>

#include <MySmartType.hpp> //моя кастомная наработка для создания умных типов данных по политикам

// --- ГЛОБАЛЬНАЯ НАСТРОЙКА ОКРУЖЕНИЯ ДЛЯ ВСЕХ ТЕСТОВ ---
// Гарантирует, что towupper узнает кириллицу до выполнения конструкторов Addresse
TEST_SUITE_BEGIN("PerformanceAndFlyweightTests");

struct GlobalLocaleSetup {
    GlobalLocaleSetup() {
        std::setlocale(LC_ALL,"ru_RU.UTF-8");
    }
} global_locale_initializer;


// --- 1. ТРЕКЕР ВЫДЕЛЕНИЯ ПАМЯТИ (Метрика памяти) ---
static size_t total_allocated_bytes = 0;

// Перегружаем глобальный оператор new, чтобы перехватывать все аллокации в куче
void* operator new(size_t size) {
    total_allocated_bytes += size;
    return malloc(size);
}
void operator delete(void* ptr) noexcept {
    free(ptr);
}

// --- 2. КЛАСС ДЛЯ ЗАМЕРА ВРЕМЕНИ (Метрика скорости) ---
struct Timer {
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::string test_name;

    Timer(std::string name) : test_name(std::move(name)) {
        start_time = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        
        // Красивый вывод метрик в консоль во время работы Doctest
        std::cout << "\n==================================================\n"
                  << "[PERF INFO] " << test_name << "\n"
                  << " -> Время выполнения: " << duration << " мкс (" << (duration / 1000.0) << " мс)\n"
                  << " -> Всего выделено памяти в куче: " << total_allocated_bytes << " байт\n"
                  << "==================================================\n" << std::endl;
    }
};


// Необходим, чтобы замерить разницу производительности нативного std::string
struct ClassicAddress {
    uint32_t m_house;
    uint32_t m_apartment;
    std::string m_city;
    std::string m_street;

    ClassicAddress(std::string c, std::string s, uint32_t h, uint32_t a)
        : m_house(h), m_apartment(a) {
        m_city = TrimAndCapitalizePolicy::process(std::move(c));
        m_street = TrimAndCapitalizePolicy::process(std::move(s));
    }
    
    std::string get_output_address() const {
        return m_city + ", " + m_street + ", " + std::to_string(m_house) + ", " + std::to_string(m_apartment);
    }
};

struct Addresse {
    uint32_t m_house;
    uint32_t m_apartment;
    CacheString m_city;   // Оптимизированная Flyweight строка (4 байта)
    CacheString m_street; // Оптимизированная Flyweight строка (4 байта)

    Addresse(std::string c, std::string s, uint32_t h, uint32_t a)
        : m_house(h), m_apartment(a), m_city(std::move(c)), m_street(std::move(s)) {}

    std::string get_output_address() const {
    // Используем штатный оператор конвертации SmartVar без жесткого приведения к ссылке &
    std::string city = m_city;
    std::string street = m_street;
    
    return city + ", " + street + ", " + 
           std::to_string(m_house) + ", " + std::to_string(m_apartment);
    }
};


// --- 3. ТЕСТОВЫЕ СЦЕНАРИИ DOCTEST ---

TEST_CASE("Сравнительное тестирование: Classic vs Flyweight Cache") {
    const int iterations = 5000;
    const int total_objects = iterations * 2;

    // 1. ЗАМЕР КЛАССИЧЕСКОГО ПОДХОДА
    total_allocated_bytes = 0;
    auto start_classic = std::chrono::high_resolution_clock::now();
    
    std::vector<ClassicAddress> classic_db;
    classic_db.reserve(total_objects);
    for (int i = 0; i < iterations; ++i) {
        classic_db.emplace_back("   москва  ", "Строителей", i, 12);
        classic_db.emplace_back(" санкт-петербург ", "Невский", i, 45);
    }
    
    auto end_classic = std::chrono::high_resolution_clock::now();
    size_t memory_classic = total_allocated_bytes;
    auto time_classic = std::chrono::duration_cast<std::chrono::microseconds>(end_classic - start_classic).count();

    // 2. ЗАМЕР ВАШЕГО СМАРТ-ВАРИАНТА (FLYWEIGHT)
    total_allocated_bytes = 0;
    auto start_smart = std::chrono::high_resolution_clock::now();
    
    std::vector<Addresse> smart_db;
    smart_db.reserve(total_objects);
    for (int i = 0; i < iterations; ++i) {
        smart_db.emplace_back("   москва  ", "Строителей", i, 12);
        smart_db.emplace_back(" санкт-петербург ", "Невский", i, 45);
    }
    
    auto end_smart = std::chrono::high_resolution_clock::now();
    size_t memory_smart = total_allocated_bytes;
    auto time_smart = std::chrono::duration_cast<std::chrono::microseconds>(end_smart - start_smart).count();

    // 3. ПРОВЕРКА КОРРЕКТНОСТИ ПОВЕДЕНИЯ ПОЛИТИКИ
    REQUIRE(smart_db.size() == total_objects);
    CHECK(smart_db[0].get_output_address().rfind("Москва", 0) == 0);
    CHECK(smart_db[1].get_output_address().rfind("Санкт-петербург", 0) == 0);

    // 4. СБОРКА И ВЫВОД ВИЗУАЛЬНОГО ОТЧЕТА
    double memory_saved_pct = 100.0 - (100.0 * memory_smart / memory_classic);
    int bar_width = 40;
    int classic_bar = bar_width;
    int smart_bar = static_cast<int>(bar_width * ((double)memory_smart / memory_classic));

    std::cout << "\n============================================================\n"
              << "       ОТЧЕТ ЭФФЕКТИВНОСТИ ПАТТЕРНА SMARTVAR FLYWEIGHT      \n"
              << "============================================================\n"
              << "Объём выборки: " << total_objects << " адресов (высокая дублируемость)\n\n"
              << "ПРОФИЛИРОВАНИЕ ПАМЯТИ (Выделено в куче):\n"
              << "  [Classic]   " << std::setw(10) << memory_classic << " байт |" 
              << std::string(classic_bar, '#') << "|\n"
              << "  [Flyweight] " << std::setw(10) << memory_smart << " байт |" 
              << std::string(smart_bar, '#') << std::string(bar_width - smart_bar, ' ') << "|\n"
              << "  [Результат] Сэкономлено: " << (memory_classic - memory_smart) 
              << " байт (" << std::fixed << std::setprecision(2) << memory_saved_pct << "%)\n\n"
              << "СКОРОСТЬ ЗАПОЛНЕНИЯ БАЗЫ:\n"
              << "  Classic:    " << time_classic << " мкс\n"
              << "  Flyweight:  " << time_smart << " мкс\n"
              << "============================================================\n" << std::endl;
}

TEST_CASE("Сравнительное тестирование ввода-вывода: Нативный vs SmartVar Flyweight") {
    const std::string input_filename = "Sorce/in.txt";
    const std::string output_classic = "Sorce/out_classic.txt";
    const std::string output_smart   = "Sorce/out_smart.txt";

    // Инициализация локали для корректного преобразования регистра внутри потоков
    std::setlocale(LC_ALL, "");

    // ============================================================
    // 1. ЗАМЕР НАТИВНОГО (КЛАССИЧЕСКОГО) ПОДХОДА
    // ============================================================
    total_allocated_bytes = 0;
    auto start_classic = std::chrono::high_resolution_clock::now();

    std::ifstream in_classic(input_filename);
    REQUIRE(in_classic.is_open()); // Тест упадет, если файла in.txt нет

    int count_classic = 0;
    in_classic >> count_classic;
    in_classic.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<ClassicAddress> classic_addresses;
    classic_addresses.reserve(count_classic);

    for (int i = 0; i < count_classic; ++i) {
        std::string city, street;
        uint32_t house, apartment;
        if (std::getline(in_classic, city) && std::getline(in_classic, street) && 
            (in_classic >> house) && (in_classic >> apartment)) {
            in_classic.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            classic_addresses.emplace_back(city, street, house, apartment);
        }
    }
    in_classic.close();

    std::ofstream out_classic_file(output_classic);
    REQUIRE(out_classic_file.is_open());
    out_classic_file << classic_addresses.size() << "\n";
    for (auto it = classic_addresses.rbegin(); it != classic_addresses.rend(); ++it) {
        out_classic_file << it->get_output_address() << "\n";
    }
    out_classic_file.close();

    auto end_classic = std::chrono::high_resolution_clock::now();
    size_t memory_classic = total_allocated_bytes;
    auto time_classic = std::chrono::duration_cast<std::chrono::microseconds>(end_classic - start_classic).count();


    // ============================================================
    // 2. ЗАМЕР ОПТИМИЗИРОВАННОГО ПОДХОДА (SMARTVAR)
    // ============================================================
    total_allocated_bytes = 0;
    auto start_smart = std::chrono::high_resolution_clock::now();

    std::ifstream in_smart(input_filename);
    REQUIRE(in_smart.is_open());

    int count_smart = 0;
    in_smart >> count_smart;
    in_smart.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<Addresse> smart_addresses;
    smart_addresses.reserve(count_smart);

    for (int i = 0; i < count_smart; ++i) {
        std::string city, street;
        uint32_t house, apartment;
        if (std::getline(in_smart, city) && std::getline(in_smart, street) && 
            (in_smart >> house) && (in_smart >> apartment)) {
            in_smart.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            smart_addresses.emplace_back(city, street, house, apartment);
        }
    }
    in_smart.close();

    std::ofstream out_smart_file(output_smart);
    REQUIRE(out_smart_file.is_open());
    out_smart_file << smart_addresses.size() << "\n";
    for (auto it = smart_addresses.rbegin(); it != smart_addresses.rend(); ++it) {
        out_smart_file << it->get_output_address() << "\n";
    }
    out_smart_file.close();

    auto end_smart = std::chrono::high_resolution_clock::now();
    size_t memory_smart = total_allocated_bytes;
    auto time_smart = std::chrono::duration_cast<std::chrono::microseconds>(end_smart - start_smart).count();


    // ============================================================
    // 3. АССЕРТЫ И ВИЗУАЛЬНЫЙ ОТЧЕТ
    // ============================================================
    CHECK(smart_addresses.size() == classic_addresses.size());

    double memory_saved_pct = (memory_classic > 0) ? (100.0 - (100.0 * memory_smart / memory_classic)) : 0.0;
    int bar_width = 40;
    int classic_bar = bar_width;
    int smart_bar = (memory_classic > 0) ? static_cast<int>(bar_width * ((double)memory_smart / memory_classic)) : 0;
    if (smart_bar > bar_width) smart_bar = bar_width; // Защита от оверфлоу аллокаций хэш-таблицы

    std::cout << "\n============================================================\n"
              << "          СРАВНИТЕЛЬНЫЙ ОТЧЕТ ФАЙЛОВОГО ВВОДА/ВЫВОДА        \n"
              << "============================================================\n"
              << "Обработано объектов: " << smart_addresses.size() << "\n\n"
              << "ПАМЯТЬ В КУЧЕ ПРИ ВВОДЕ/ВЫВОДЕ (Включая буферы потоков):\n"
              << "  [Native]    " << std::setw(10) << memory_classic << " байт |" 
              << std::string(classic_bar, '#') << "|\n"
              << "  [SmartVar]  " << std::setw(10) << memory_smart << " байт |" 
              << std::string(smart_bar, '#') << std::string(bar_width - smart_bar, ' ') << "|\n"
              << "  [Результат] Сэкономлено: " << (static_cast<long long>(memory_classic) - static_cast<long long>(memory_smart)) 
              << " байт (" << std::fixed << std::setprecision(2) << memory_saved_pct << "%)\n\n"
              << "ОБЩЕЕ ВРЕМЯ (Диск -> ОЗУ -> Трансформация -> Диск):\n"
              << "  Native System: " << time_classic << " мкс\n"
              << "  SmartVar Sys:  " << time_smart << " мкс\n"
              << "============================================================\n" << std::endl;
}