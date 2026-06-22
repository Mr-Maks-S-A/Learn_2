/**
 * @file main.cpp
 * @brief Главный файл программы 
 */
#include <iostream>
#include <string>
#include <exception>

// собственное исключение, наследуясь от std::exception
class bad_length : public std::exception {
public:
    const char* what() const noexcept override {
        return "Вы ввели слово запретной длины! До свидания";
    }
};

// Функция проверки длины строки
int function(std::string str, int forbidden_length) {
    // В C++ str.length() возвращает количество байт. 
    // Для кириллицы в UTF-8 один символ занимает больше 1 байта, 
    int current_length = str.length(); 
    
    if (current_length == forbidden_length) {
        throw bad_length(); // Выбрасываем исключение
    }
    
    return current_length;
}

int main() {
    std::setlocale(LC_ALL, "Russian");
    
    int forbidden_length = 0;
    std::cout << "Введите запретную длину: ";
    std::cin >> forbidden_length;
    
    std::string user_word;
    
    while (true) {
        std::cout << "Введите слово: ";
        std::cin >> user_word;
        
        try {
            int length = function(user_word, forbidden_length);
            std::cout << "Длина слова \"" << user_word << "\" равна " << length << std::endl;
        }
        catch (const bad_length& e) {
            std::cout << e.what() << std::endl;
            break; 
        }
    }
    
    return EXIT_SUCCESS;
};