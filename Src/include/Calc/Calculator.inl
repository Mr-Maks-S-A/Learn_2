#include <stdexcept>
#include <cctype>
template <ArithmeticNumber T>
T Calculator<T>::evaluate(const std::string &expr) {
    std::istringstream input(expr);
    pos = &input;
    
    T result = parseExpression();
    
    skipSpaces();
    if (pos->peek() != EOF) {
        throw std::runtime_error("Обнаружены лишние символы в конце выражения");
    }
    
    return result;
}

template <ArithmeticNumber T>
T Calculator<T>::parseExpression() {
    T value = parseTerm();
    while (true) {
        char op = peek();
        if (op == '+' || op == '-') {
            get(); 
            T rhs = parseTerm();
            if (op == '+') value += rhs;
            else value -= rhs;
        } else break;
    }
    return value;
}

template <ArithmeticNumber T>
T Calculator<T>::parseTerm() {
    // Теперь спускаемся не к фактору, а к уровню степеней
    T value = parsePower(); 
    while (true) {
        char op = peek();
        if (op == '*' || op == '/') {
            get(); 
            T rhs = parsePower();
            if (op == '*') {
                value *= rhs;
            } else {
                if (rhs == static_cast<T>(0)) {
                    throw std::runtime_error("Деление на ноль");
                }
                
                if constexpr (std::is_integral_v<T>) {
                    value /= rhs;
                } else {
                    value /= rhs;
                }
            }
        } else break;
    }
    return value;
}

template <ArithmeticNumber T>
T Calculator<T>::parsePower() {
    T value = parseFactor();
    
    if (peek() == '^') {
        get(); // Поглощаем оператор '^'
        // Рекурсивный вызов parsePower() вместо цикла обеспечивает правую ассоциативность
        T rhs = parsePower(); 
        
        if constexpr (std::is_integral_v<T>) {
            value = ipow(value, rhs);
        } else {
            value = std::pow(value, rhs);
        }
    }
    
    return value;
}

template <ArithmeticNumber T>
T Calculator<T>::parseFactor() {
    skipSpaces();
    char c = peek();
    if (c == '(') {
        get(); 
        T val = parseExpression();
        if (get() != ')') throw std::runtime_error("Ожидалась закрывающая скобка ')'");
        return val;
    } 
    return parseNumber();
}

template <ArithmeticNumber T>
T Calculator<T>::parseNumber() {
    skipSpaces();
    T val;
    if (!(*pos >> val)) throw std::runtime_error("Ошибка чтения числа");
    return val;
}

template <ArithmeticNumber T>
char Calculator<T>::peek() {
    skipSpaces();
    return pos->peek();
}

template <ArithmeticNumber T>
char Calculator<T>::get() {
    skipSpaces();
    return pos->get();
}

template <ArithmeticNumber T>
void Calculator<T>::skipSpaces() {
    while (std::isspace(pos->peek())) {
        pos->get();
    }
}