#include <stdexcept>
#include <string>


#if defined(__GNUC__) || defined(__clang__)
    #define CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
    #define CURRENT_FUNCTION __FUNCSIG__
#else
    #define CURRENT_FUNCTION __func__ 
#endif

class figure_error : public std::domain_error {
public:
    explicit figure_error(const std::string& message, 
                         const std::string& function_name, 
                         const std::string& file, 
                         int line) 
        : std::domain_error(
            "Файл: " + file + " (строка " + std::to_string(line) + ")\n" +
            "Функция: " + function_name + "\n" +
            "Ошибка: " + message
          ) {}
};

#define THROW_FIGURE_ERROR(msg) throw figure_error(msg, CURRENT_FUNCTION, __FILE__, __LINE__)



#ifdef SHUTDOWN_ON_ERROR
    #define HANDLE_CONSTRUCTOR_ERROR(ex) \
        std::cerr << "[КРИТИКА] Срочная остановка программы!\n" << ex.what() << "\n\n"; \
        std::terminate();
#else
    #define HANDLE_CONSTRUCTOR_ERROR(ex) \
        std::cerr << "[ЛОГ КОНСТРУКТОРА] Попытка выстоять. Исключение улетает выше:\n" << ex.what() << "\n\n";
#endif
