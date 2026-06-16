#include <iostream>
#include <sstream>
#include <string>
#include <cassert>

// Подключаем ваш заголовочный файл
#include <Until/Input.hpp> 

// Красивый макрос для тестов: если падает, то показывает детали
#define ASSERT_EQUAL(actual, expected) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "Крах теста в строке " << __LINE__ \
                      << ": получено '" << (actual) \
                      << "', ожидалось '" << (expected) << "'\n"; \
            std::abort(); \
        } \
    } while(false)

// RAII-класс для подмены std::cin
class RedirectCin {
public:
    explicit RedirectCin(std::streambuf* new_buffer) 
        : old_buffer(std::cin.rdbuf(new_buffer)) {}
    ~RedirectCin() { std::cin.rdbuf(old_buffer); }
private:
    std::streambuf* old_buffer;
};

// RAII-класс для ПЕРЕХВАТА std::cout (теперь мы можем проверять текст ошибок!)
class CaptureCout {
public:
    CaptureCout() : old_buffer(std::cout.rdbuf(capture_stream.rdbuf())) {}
    ~CaptureCout() { std::cout.rdbuf(old_buffer); }
    std::string get_output() const { return capture_stream.str(); }
    void clear() { capture_stream.str(""); capture_stream.clear(); }
private:
    std::stringstream capture_stream;
    std::streambuf* old_buffer;
};

// --- УЛУЧШЕННЫЕ ТЕСТЫ ---

void test_successful_int_input() {
    std::istringstream mock_input("42\n");
    RedirectCin redirect(mock_input.rdbuf());

    int result = get_input<int>("Ввод: ");
    ASSERT_EQUAL(result, 42);
}

void test_float_input_with_spaces() {
    std::istringstream mock_input("   3.14\n");
    RedirectCin redirect(mock_input.rdbuf());

    double result = get_input<double>("Ввод: ");
    ASSERT_EQUAL(result, 3.14);
}

void test_range_and_error_output() {
    // 150 (ошибка диапазона), затем 50 (успех)
    std::istringstream mock_input("150\n50\n");
    RedirectCin redirect(mock_input.rdbuf());
    
    CaptureCout capture; 
    int result = get_input<int>("Ввод: ", 0, 100);
    
    ASSERT_EQUAL(result, 50);
    // Проверяем, что пользователю действительно вывелась ошибка диапазона
    assert(capture.get_output().find("Ошибка: значение должно быть в диапазоне") != std::string::npos);
}

void test_invalid_chars_handling() {
    // "abc" (мусор), затем "10" (успех)
    std::istringstream mock_input("abc\n10\n");
    RedirectCin redirect(mock_input.rdbuf());

    CaptureCout capture;
    int result = get_input<int>("Ввод: ");
    
    ASSERT_EQUAL(result, 10);
    assert(capture.get_output().find("Ошибка: введите корректное число") != std::string::npos);
}

void test_exclude_list() {
    // 5 (в черном списке), затем 7 (разрешено)
    std::istringstream mock_input("5\n7\n");
    RedirectCin redirect(mock_input.rdbuf());

    CaptureCout capture;
    int result = get_input<int>("Ввод: ", 0, 10, {5, 6});
    
    ASSERT_EQUAL(result, 7);
    assert(capture.get_output().find("находится в списке запрещенных") != std::string::npos);
}

void test_include_list_validation() {
    // Тестируем перегрузку с белым списком. 
    // Сначала вводим 4 (нет в списке), затем 2 (есть в списке)
    std::istringstream mock_input("4\n2\n");
    RedirectCin redirect(mock_input.rdbuf());

    CaptureCout capture;
    int result = get_input<int>("Ввод: ", {1, 2, 3});

    ASSERT_EQUAL(result, 2);
    assert(capture.get_output().find("Ошибка: недопустимый ввод. Разрешено только") != std::string::npos);
}

void test_string_input() {
    std::istringstream mock_input("Hello_World\n");
    RedirectCin redirect(mock_input.rdbuf());

    std::string result = get_input<std::string>("Ввод: ");
    ASSERT_EQUAL(result, "Hello_World");
}

void test_string_length_validation() {
    // "hi" (короткая), затем "ValidString" (нормальная)
    std::istringstream mock_input("hi\nValidString\n");
    RedirectCin redirect(mock_input.rdbuf());

    CaptureCout capture;
    std::string result = get_input<std::string>("Ввод: ", 5, 20);
    
    ASSERT_EQUAL(result, "ValidString");
    assert(capture.get_output().find("Ошибка: длина строки должна быть от") != std::string::npos);
}

int main() {
    std::cout << "=======================================\n";
    std::cout << " Запуск улучшенных тестов get_input...\n";
    std::cout << "=======================================\n";

    test_successful_int_input();
    std::cout << "[ОК] Успешный ввод целого числа\n";

    test_float_input_with_spaces();
    std::cout << "[ОК] Успешный ввод чисел с пробелами\n";

    test_range_and_error_output();
    std::cout << "[ОК] Валидация диапазонов и вывод ошибок\n";

    test_invalid_chars_handling();
    std::cout << "[ОК] Обработка некорректных символов (строк)\n";

    test_exclude_list();
    std::cout << "[ОК] Проверка работы списка исключений (exclude_list)\n";

    test_include_list_validation();
    std::cout << "[ОК] Проверка работы белого списка (include_list)\n";

    test_string_input();
    std::cout << "[ОК] Базовый ввод строк\n";

    test_string_length_validation();
    std::cout << "[ОК] Проверка ограничений длины строки\n";

    std::cout << "---------------------------------------\n";
    std::cout << "ВСЕ ТЕСТЫ ВЫПОЛНЕНЫ УСПЕШНО!\n";
    return 0;
}