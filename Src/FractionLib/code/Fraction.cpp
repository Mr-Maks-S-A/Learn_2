#include <Fraction/Fraction.hpp>
#include <cmath>

// Приватный метод для поиска НОД (Алгоритм Евклида)
int Fraction::gcd(int numerator, int denominator) const {
    numerator = std::abs(numerator);
    denominator = std::abs(denominator);
    while (denominator != 0) {
        int temp = denominator;
        denominator = numerator % denominator;
        numerator = temp;
    }
    return numerator;
};

// Вспомогательный приватный метод для сокращения дроби и работы со знаками
void Fraction::reduce() {
    // 1. Обработка деления на ноль
    if (denominator == 0) {
        throw std::invalid_argument("Знаменатель не может быть равен нулю.");
    }
    
    // 2. Поиск НОД
    int g = gcd(numerator, denominator);
    
    // 3. Сокращаем
    numerator /= g;
    denominator /= g;
    
    // 4. Следим, чтобы минус был только в числителе
    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }
};

// Конструктор
Fraction::Fraction(int num, int denom) : numerator(num), denominator(denom) {
    reduce();
};

// Метод dump для тестов
std::string Fraction::dump() const {
    return std::to_string(numerator) + "/" + std::to_string(denominator);
};




// Сложение: a/b + c/d = (a*d + c*b) / (b*d)
Fraction Fraction::operator+(const Fraction& other) const {
    int num = numerator * other.denominator + other.numerator * denominator;
    int denom = denominator * other.denominator;
    return Fraction(num, denom);
};

// Вычитание: a/b - c/d = (a*d - c*b) / (b*d)
Fraction Fraction::operator-(const Fraction& other) const {
    int num = numerator * other.denominator - other.numerator * denominator;
    int denom = denominator * other.denominator;
    return Fraction(num, denom);
};

// Умножение: a/b * c/d = (a*c) / (b*d)
Fraction Fraction::operator*(const Fraction& other) const {
    int num = numerator * other.numerator;
    int denom = denominator * other.denominator;
    return Fraction(num, denom);
};

// Деление: a/b / c/d = (a*d) / (b*c)
Fraction Fraction::operator/(const Fraction& other) const {
    if (other.numerator == 0) {
        throw std::invalid_argument("Деление на ноль: числитель делителя равен нулю.");
    }
    int num = numerator * other.denominator;
    int denom = denominator * other.numerator;
    return Fraction(num, denom);
};





// Так как в конструкторе всегда вызывается reduce(), дроби уникальны.
// Можно просто сравнить числители и знаменатели.
bool Fraction::operator==(const Fraction& other) const {
    return (numerator == other.numerator) && (denominator == other.denominator);
};

bool Fraction::operator!=(const Fraction& other) const {
    return !(*this == other);
};

// Сравнение «крест-накрест»: a/b < c/d  =>  a*d < c*b
bool Fraction::operator<(const Fraction& other) const {
    return (numerator * other.denominator) < (other.numerator * denominator);
};

bool Fraction::operator>(const Fraction& other) const {
    return other < *this;
};

bool Fraction::operator<=(const Fraction& other) const {
    return !(*this > other);
};

bool Fraction::operator>=(const Fraction& other) const {
    return !(*this < other);
};




Fraction Fraction::operator-() const {
    return Fraction(-numerator, denominator);
};

Fraction Fraction::operator+() const {
    return *this; // Просто возвращает копию текущего объекта без изменений
};





// Префиксный инкремент (++f): изменяет сам объект, возвращает ссылку
Fraction& Fraction::operator++() {
    numerator += denominator;
    return *this;
};

// Постфиксный инкремент (f++): возвращает старую копию, а оригинал увеличивает
Fraction Fraction::operator++(int) {
    Fraction temp = *this;
    ++(*this); // Вызываем уже написанный префиксный инкремент
    return temp;
};


// Префиксный декремент (--f): изменяет сам объект, возвращает ссылку
Fraction& Fraction::operator--() {
    numerator -= denominator;
    return *this;
};

// Постфиксный декремент (f--): возвращает старую копию, а оригинал уменьшает
Fraction Fraction::operator--(int) {
    Fraction temp = *this;
    --(*this); // Вызываем уже написанный префиксный декремент
    return temp;
};