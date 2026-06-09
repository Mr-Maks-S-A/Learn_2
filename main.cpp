#include <cstdint>
#include <iostream> 
#include <locale> 
#include <string>

#include <MyRedererOOP.hpp> 


// --- Компактные структуры данных ---
struct Sides  { int a = 0, b = 0, c = 0, d = 0; };
struct Angles { int A = 0, B = 0, C = 0, D = 0; };

// --- Базовый класс Фигура ---
class Figure {
protected:
    std::string name;
    Sides s;
    Angles n;
    int sides_count = 0;
    bool is_quad = false;

public:
    Figure(const std::string& name = "Фигура") : name(name) {}
    virtual ~Figure() = default;

    std::string get_name() const { return name; }
    int get_sides_count() const { return sides_count; }
    const Sides& get_sides() const { return s; }
    const Angles& get_angles() const { return n; }

    virtual void print_sides() const {
        if (sides_count == 0) return;
        std::cout << "Стороны: a=" << s.a << " b=" << s.b << " c=" << s.c;
        if (is_quad) std::cout << " d=" << s.d;
        std::cout << "\n";
    }
    
    virtual void print_angles() const {
        if (sides_count == 0) return;
        std::cout << "Углы: A=" << n.A << " B=" << n.B << " C=" << n.C;
        if (is_quad) std::cout << " D=" << n.D;
        std::cout << "\n";
    }

    virtual void render_hierarchy() const { render_class_line<Figure>("Figure"); }
};

// --- ВЕТКА ТРЕУГОЛЬНИКОВ ---

class Triangle : public Figure {
public:
    Triangle(int a, int b, int c, int A, int B, int C, const std::string& name = "Треугольник") : Figure(name) {
        s = {a, b, c}; n = {A, B, C}; sides_count = 3;
    }
    void render_hierarchy() const override { Figure::render_hierarchy(); render_class_line<Triangle>("Triangle"); }
};

class RightTriangle : public Triangle {
public:
    RightTriangle(int a, int b, int c, int A, int B) : Triangle(a, b, c, A, B, 90, "Прямоугольный треугольник") {}
    void render_hierarchy() const override { Triangle::render_hierarchy(); render_class_line<RightTriangle>("RightTriangle"); }
};

class IsoscelesTriangle : public Triangle {
public:
    IsoscelesTriangle(int a, int b, int A, int B) : Triangle(a, b, a, A, B, A, "Равнобедренный треугольник") {}
    void render_hierarchy() const override { Triangle::render_hierarchy(); render_class_line<IsoscelesTriangle>("IsoscelesTriangle"); }
};

class EquilateralTriangle : public Triangle {
public:
    EquilateralTriangle(int side) : Triangle(side, side, side, 60, 60, 60, "Равносторонний треугольник") {}
    void render_hierarchy() const override { Triangle::render_hierarchy(); render_class_line<EquilateralTriangle>("EquilateralTriangle"); }
};

// --- ВЕТКА ЧЕТЫРЕХУГОЛЬНИКОВ ---

class Quadrangle : public Figure {
public:
    Quadrangle(int a, int b, int c, int d, int A, int B, int C, int D, const std::string& name = "Четырёхугольник") : Figure(name) {
        s = {a, b, c, d}; n = {A, B, C, D}; sides_count = 4; is_quad = true;
    }
    void render_hierarchy() const override { Figure::render_hierarchy(); render_class_line<Quadrangle>("Quadrangle"); }
};

class Parallelogram : public Quadrangle {
public:
    Parallelogram(int a, int b, int A, int B, const std::string& name = "Параллелограмм") : Quadrangle(a, b, a, b, A, B, A, B, name) {}
    void render_hierarchy() const override { Quadrangle::render_hierarchy(); render_class_line<Parallelogram>("Parallelogram"); }
};

class RectangleFigure : public Parallelogram {
public:
    RectangleFigure(int a, int b, const std::string& name = "Прямоугольник") : Parallelogram(a, b, 90, 90, name) {}
    void render_hierarchy() const override { Parallelogram::render_hierarchy(); render_class_line<RectangleFigure>("RectangleFigure"); }
};

class Rhombus : public Parallelogram {
public:
    Rhombus(int a, int A, int B, const std::string& name = "Ромб") : Parallelogram(a, a, A, B, name) {}
    void render_hierarchy() const override { Parallelogram::render_hierarchy(); render_class_line<Rhombus>("Rhombus"); }
};

class Square : public RectangleFigure {
public:
    Square(int a) : RectangleFigure(a, a, "Квадрат") {}
    void render_hierarchy() const override { RectangleFigure::render_hierarchy(); render_class_line<Square>("Square"); }
};

// ==========================================
// --- РЕГИСТРАЦИЯ КОМПИЛЯТОРНЫХ МЕТАДАННЫХ --
// ==========================================

REGISTER_PARENT(Triangle, Figure)
  REGISTER_PARENT(RightTriangle, Triangle)
  REGISTER_PARENT(IsoscelesTriangle, Triangle)
  REGISTER_PARENT(EquilateralTriangle, Triangle)

REGISTER_PARENT(Quadrangle, Figure)
  REGISTER_PARENT(Parallelogram, Quadrangle)
    REGISTER_PARENT(RectangleFigure, Parallelogram)
      REGISTER_PARENT(Square, RectangleFigure)
    REGISTER_PARENT(Rhombus, Parallelogram)


// --- Функция вывода информации ---
void print_info(const Figure* fig) {
    if (!fig) return;
    std::cout << fig->get_name() << " (Сторон: " << fig->get_sides_count() << "):\n";
    fig->print_sides();
    fig->print_angles();
    std::cout << "Иерархия типа:\n";
    fig->render_hierarchy();
    std::cout << "\n-----------------------------------------\n";
}

int main() {
    std::setlocale(LC_ALL, "Russian");

    Figure generic_figure;
    Triangle triangle(10, 20, 30, 50, 60, 70);
    RightTriangle r_triangle(10, 20, 30, 50, 60);
    IsoscelesTriangle i_triangle(10, 20, 50, 60);
    EquilateralTriangle e_triangle(30);

    Quadrangle quadrangle(10, 20, 30, 40, 50, 60, 70, 80);
    Parallelogram parallelogram(20, 30, 30, 40);
    RectangleFigure rectangle(10, 20);
    Rhombus rhombus(30, 30, 40);
    Square square(20);

    const Figure* figures[] = { 
        &generic_figure, &triangle, &r_triangle, &i_triangle, &e_triangle, 
        &quadrangle, &parallelogram, &rectangle, &rhombus, &square 
    };

    for (const auto* fig : figures) {
        print_info(fig);
    }
    
    return EXIT_SUCCESS;
}