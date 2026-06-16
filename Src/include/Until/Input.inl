template <ArithmeticNumber T>
T get_input(const std::string& prompt,
            T min_val,
            T max_val,
            std::initializer_list<T> exclude_list) {
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
        
        if (value < min_val || value > max_val) {
            std::cout << "Ошибка: значение должно быть в диапазоне от "
                      << min_val << " до " << max_val << ".\n";
            continue;
        }
        
        if (exclude_list.size() > 0) {
            if (std::ranges::any_of(exclude_list, [value](T excluded) { return value == excluded; })) {
                std::cout << "Ошибка: значение " << value << " находится в списке запрещенных.\n";
                continue;
            }
        }
        
        return value;
    }
}

template <typename T>
T get_input(const std::string& prompt,
            std::initializer_list<T> include_list) {
    T value;
    while (true) {
        std::cout << prompt;
        while (std::isspace(std::cin.peek()) && std::cin.peek() != '\n') {
            std::cin.get();
        }
        if constexpr (std::is_unsigned_v<T>) {
            if (std::cin.peek() == '-') {
                std::cout << "Ошибка: введено отрицательное число.\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (!std::getline(std::cin >> std::ws, value)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка ввода строки.\n";
                continue;
            }
        } else {
            if (!(std::cin >> value)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Ошибка: неверный формат данных.\n";
                continue;
            }
            while (std::isspace(std::cin.peek()) && std::cin.peek() != '\n') {
                std::cin.get();
            }
            if (std::cin.peek() != '\n' && std::cin.peek() != EOF) {
                std::cout << "Ошибка: обнаружены лишние символы.\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            std::cin.ignore();
        }
        if (std::ranges::any_of(include_list, [value](const T& allowed) { return value == allowed; })) {
            return value;
        }
        std::cout << "Ошибка: недопустимый ввод. Разрешено только: ";
        for (const auto& item : include_list) {
            std::cout << "'" << item << "' ";
        }
        std::cout << "\n";
    }
}

template <typename T>
requires std::is_same_v<T, std::string>
std::string get_input(const std::string& prompt
                                 ,size_t min_len
                                 ,size_t max_len) {
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
};

template <typename T>
requires std::is_same_v<T, std::string>
std::string get_input(const std::string& prompt,
                      auto validator,
                      const std::string& error_msg)
    requires std::predicate<decltype(validator), const std::string&> 
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

        if (value == "q" || value == "Q") {
            return value;
        }

        if (!validator(value)) {
            std::cout << error_msg;
            continue;
        }

        return value;
    }
}