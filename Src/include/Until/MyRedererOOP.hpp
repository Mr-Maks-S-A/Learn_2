#pragma once
#include <iostream>
#include <string>
#include <type_traits>

// Магия метапрограммирования 
// и моя попытка понять значимость рефлексии которой нет

// Базовый шаблон для определения родителя
template <typename T>
struct BaseOf {
    using Type = void; // По умолчанию родителя нет
};

// Вспомогательный макрос, чтобы не писать много шаблонного кода вручную
#define REGISTER_PARENT(Child, Parent) \
template <> struct BaseOf<Child> { using Type = Parent; };

// Функция, которая считает глубину наследования в compile-time
template <typename T>
constexpr int get_depth() {
    if constexpr (std::is_same_v<typename BaseOf<T>::Type, void>) {
        return 0;
    } else {
        return 1 + get_depth<typename BaseOf<T>::Type>();
    }
}

// Шаблонный рендер одной ветки иерархии (снизу вверх)
template <typename T>
void render_class_line(const std::string& current_name) {
    int depth = get_depth<T>();
    
    // Рисуем отступы в зависимости от глубины класса в дереве
    for (int i = 0; i < depth; ++i) {
        std::cout << "  |";
    }
    if (depth > 0) std::cout << "-- ";
    
    std::cout << current_name << "\n";
}


//============================пример============================


// class A {
// public:
//     virtual ~A() = default;
//     // Каждый класс просто рендерит себя, передавая свой тип в шаблон
//     virtual void render_hierarchy() const { render_class_line<A>("A"); }
// };

// class B : public A {
// public:
//     void render_hierarchy() const override {
//         // Сначала просим родителя нарисовать верхнюю часть дерева
//         A::render_hierarchy(); 
//         // Затем рисуем себя
//         render_class_line<B>("B");
//     }
// };
// REGISTER_PARENT(B, A) // Регистрируем связь для метаданных

// class B2 : public A {
// public:
//     void render_hierarchy() const override {
//         A::render_hierarchy();
//         render_class_line<B2>("B2");
//     }
// };
// REGISTER_PARENT(B2, A)

// class C : public B2 {
// public:
//     void render_hierarchy() const override {
//         B2::render_hierarchy();
//         render_class_line<C>("C");
//     }
// };
// REGISTER_PARENT(C, B2)

// int main() {
//     std::cout << "--- Рендер для объекта класса C ---\n";
//     C objC;
//     objC.render_hierarchy();

//     std::cout << "\n--- Рендер для объекта класса B2 ---\n";
//     B2 objB2;
//     objB2.render_hierarchy();

//     return 0;
// }