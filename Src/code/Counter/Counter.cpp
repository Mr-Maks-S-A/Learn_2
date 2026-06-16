#include <Counter/Counter.hpp>
#include <iostream>

// Конструктор по умолчанию
Counter::Counter() : m_value(1) {
    std::cout << "Вызван счётчик по умолчанию >> значение равно = " << m_value << std::endl;
}

// Конструктор с параметром
Counter::Counter(int initial_value) : m_value(initial_value) {
    std::cout << "Вызван счётчик c параметром >> значение равно = " << m_value << std::endl;
}

// Метод увеличения на 1
void Counter::increment() {
    m_value++;
}

// Метод уменьшения на 1
void Counter::decrement() {
    m_value--;
}

// Метод просмотра текущего значения
int Counter::get_value() const {
    return m_value;
}