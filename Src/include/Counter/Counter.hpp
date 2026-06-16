#ifndef COUNTER_H
#define COUNTER_H

class Counter {
private:
    int m_value;

public:
    // Конструкторы
    Counter();
    Counter(int initial_value);
    
    // Деструктор
    ~Counter() = default;

    // Методы (const-модификатор остается там, где состояние не меняется)
    void increment();
    void decrement();
    int get_value() const;
};

#endif // COUNTER_H