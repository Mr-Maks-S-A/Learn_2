#include <cstdint>
#include <iostream> 
#include <locale> 
#include <string>

#include <MyRedererOOP.hpp> 


// Базовый класс для всех фигур
class Figure {
protected:
    int sides_count;
    std::string name;

public:
    // Конструктор по умолчанию для неизвестной фигуры
    Figure() : sides_count(0), name("Фигура") {}

    // Геттер для получения количества сторон
    int get_sides_count() const {
        return sides_count;
    }

    // Геттер для получения названия фигуры
    std::string get_name() const {
        return name;
    }
    virtual ~Figure() = default;
    // Каждый класс просто рендерит себя, передавая свой тип в шаблон
    virtual void render_hierarchy() const { render_class_line<Figure>("Figure"); }
};


// Класс Треугольник, наследуется от Figure
class Triangle : public Figure {
public:
    Triangle() {
        sides_count = 3;
        name = "Треугольник";
    }
    void render_hierarchy() const override {
        // Сначала просим родителя нарисовать верхнюю часть дерева
        Figure::render_hierarchy(); 
        // Затем рисуем себя
        render_class_line<Triangle>("Triangle");
    }
};

// Класс Четырёхугольник, наследуется от Figure
class Quadrangle : public Figure {
public:
    Quadrangle() {
        sides_count = 4;
        name = "Четырёхугольник";
    }
    void render_hierarchy() const override {
            // Сначала просим родителя нарисовать верхнюю часть дерева
            Figure::render_hierarchy(); 
            // Затем рисуем себя
            render_class_line<Quadrangle>("Quadrangle");
        }
};


// Регистрируем связи для метаданных
REGISTER_PARENT(Triangle, Figure)
REGISTER_PARENT(Quadrangle, Figure)

int main() {
    // Настройка локализации для корректного вывода кириллицы в консоли
    std::setlocale(LC_ALL, "Russian");

    // Создаем экземпляры каждого класса
    Figure generic_figure;
    Triangle triangle;
    Quadrangle quadrangle;

    // Выводим информацию на консоль
    std::cout << "Количество сторон:\n";
    std::cout << generic_figure.get_name() << ": " << generic_figure.get_sides_count() << "\n";
    std::cout << triangle.get_name() << ": " << triangle.get_sides_count() << "\n";
    std::cout << quadrangle.get_name() << ": " << quadrangle.get_sides_count() << "\n\n\n";

    std::cout << "--- Рендер Иерархии для объекта класса Figure ---\n";
    generic_figure.render_hierarchy();

    std::cout << "\n\n--- Рендер Иерархии для объекта класса Triangle ---\n";
    triangle.render_hierarchy();

    std::cout << "\n\n--- Рендер Иерархии для объекта класса Quadrangle ---\n";
    quadrangle.render_hierarchy();
    
    return EXIT_SUCCESS;
}