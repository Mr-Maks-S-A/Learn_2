#include <cstdint>
#include <iostream> 
#include <locale> 
#include <string>

#include <MyRedererOOP.hpp> 
#include <iostream> 
#include <locale> 
#include <string>
#include <vector>

// Если MyRedererOOP.hpp нужен для компиляции, раскомментируй строку ниже:
// #include <MyRedererOOP.hpp> 

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

    // Проверка правильности (для базовой фигуры сторон должно быть 0)
    virtual bool check() const {
        return sides_count == 0;
    }

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

    // Виртуальный метод вывода информации по ТЗ
    virtual void print_info() const {
        std::cout << name << ":\n";
        std::cout << (check() ? "Правильная" : "Неправильная") << "\n";
        std::cout << "Количество сторон: " << sides_count << "\n";
        print_sides();
        print_angles();
        std::cout << "Иерархия типа :\n";
        render_hierarchy();
    }

    virtual void render_hierarchy() const { 
        render_class_line<Figure>("Figure"); 
    }
};

// --- ВЕТКА ТРЕУГОЛЬНИКОВ ---

class Triangle : public Figure {
public:
    Triangle(int a, int b, int c, int A, int B, int C, const std::string& name = "Треугольник") 
        : Figure(name) {
        s = {a, b, c}; n = {A, B, C}; sides_count = 3;
    }

    bool check() const override {
        return (sides_count == 3) && ((n.A + n.B + n.C) == 180);
    }

    void render_hierarchy() const override { 
        Figure::render_hierarchy(); 
        render_class_line<Triangle>("Triangle"); 
    }
};

class RightTriangle : public Triangle {
public:
    RightTriangle(int a, int b, int c, int A, int B, int C = 90) 
        : Triangle(a, b, c, A, B, C, "Прямоугольный треугольник") {}

    bool check() const override {
        return Triangle::check() && (n.C == 90);
    }

    void render_hierarchy() const override { 
        Triangle::render_hierarchy(); 
        render_class_line<RightTriangle>("RightTriangle"); 
    }
};

class IsoscelesTriangle : public Triangle {
public:
    IsoscelesTriangle(int a, int b, int c, int A, int B, int C) 
        : Triangle(a, b, c, A, B, C, "Равнобедренный треугольник") {}

    bool check() const override {
        return Triangle::check() && (s.a == s.c) && (n.A == n.C);
    }

    void render_hierarchy() const override { 
        Triangle::render_hierarchy(); 
        render_class_line<IsoscelesTriangle>("IsoscelesTriangle"); 
    }
};

class EquilateralTriangle : public Triangle {
public:
    EquilateralTriangle(int a, int b, int c, int A, int B, int C) 
        : Triangle(a, b, c, A, B, C, "Равносторонний треугольник") {}

    bool check() const override {
        return Triangle::check() && 
               (s.a == s.b && s.b == s.c) && 
               (n.A == 60 && n.B == 60 && n.C == 60);
    }

    void render_hierarchy() const override { 
        Triangle::render_hierarchy(); 
        render_class_line<EquilateralTriangle>("EquilateralTriangle"); 
    }
};

// --- ВЕТКА ЧЕТЫРЕХУГОЛЬНИКОВ ---

class Quadrangle : public Figure {
public:
    Quadrangle(int a, int b, int c, int d, int A, int B, int C, int D, const std::string& name = "Четырёхугольник") 
        : Figure(name) {
        s = {a, b, c, d}; n = {A, B, C, D}; sides_count = 4; is_quad = true;
    }

    bool check() const override {
        return (sides_count == 4) && ((n.A + n.B + n.C + n.D) == 360);
    }

    void render_hierarchy() const override { 
        Figure::render_hierarchy(); 
        render_class_line<Quadrangle>("Quadrangle"); 
    }
};

class Parallelogram : public Quadrangle {
public:
    Parallelogram(int a, int b, int c, int d, int A, int B, int C, int D, const std::string& name = "Параллелограмм") 
        : Quadrangle(a, b, c, d, A, B, C, D, name) {}

    bool check() const override {
        return Quadrangle::check() && (s.a == s.c && s.b == s.d) && (n.A == n.C && n.B == n.D);
    }

    void render_hierarchy() const override { 
        Quadrangle::render_hierarchy(); 
        render_class_line<Parallelogram>("Parallelogram"); 
    }
};

class RectangleFigure : public Parallelogram {
public:
    RectangleFigure(int a, int b, int c, int d, int A, int B, int C, int D, const std::string& name = "Прямоугольник") 
        : Parallelogram(a, b, c, d, A, B, C, D, name) {}

    bool check() const override {
        return Parallelogram::check() && (n.A == 90 && n.B == 90 && n.C == 90 && n.D == 90);
    }

    void render_hierarchy() const override { 
        Parallelogram::render_hierarchy(); 
        render_class_line<RectangleFigure>("RectangleFigure"); 
    }
};

class Rhombus : public Parallelogram {
public:
    Rhombus(int a, int b, int c, int d, int A, int B, int C, int D, const std::string& name = "Ромб") 
        : Parallelogram(a, b, c, d, A, B, C, D, name) {}

    bool check() const override {
        return Parallelogram::check() && (s.a == s.b && s.b == s.c && s.c == s.d);
    }

    void render_hierarchy() const override { 
        Parallelogram::render_hierarchy(); 
        render_class_line<Rhombus>("Rhombus"); 
    }
};

class Square : public RectangleFigure {
public:
    Square(int a, int b, int c, int d, int A, int B, int C, int D) 
        : RectangleFigure(a, b, c, d, A, B, C, D, "Квадрат") {}

    bool check() const override {
        return RectangleFigure::check() && (s.a == s.b && s.b == s.c && s.c == s.d);
    }

    void render_hierarchy() const override { 
        RectangleFigure::render_hierarchy(); 
        render_class_line<Square>("Square"); 
    }
};

REGISTER_PARENT(Triangle, Figure)
  REGISTER_PARENT(RightTriangle, Triangle)
  REGISTER_PARENT(IsoscelesTriangle, Triangle)
  REGISTER_PARENT(EquilateralTriangle, Triangle)

REGISTER_PARENT(Quadrangle, Figure)
  REGISTER_PARENT(Parallelogram, Quadrangle)
    REGISTER_PARENT(RectangleFigure, Parallelogram)
      REGISTER_PARENT(Square, RectangleFigure)
    REGISTER_PARENT(Rhombus, Parallelogram)


int main() {
    std::setlocale(LC_ALL, "Russian");

    std::vector<Figure*> figures;

    figures.push_back(new Figure());
    figures.push_back(new Triangle(10, 20, 30, 50, 60, 70));
    figures.push_back(new RightTriangle(10, 20, 30, 50, 60, 90));
    figures.push_back(new RightTriangle(10, 20, 30, 50, 40, 90));
    figures.push_back(new IsoscelesTriangle(10, 20, 10, 50, 60, 50));
    figures.push_back(new EquilateralTriangle(30, 30, 30, 60, 60, 60));

    figures.push_back(new Quadrangle(10, 20, 30, 40, 50, 60, 70, 80));
    figures.push_back(new RectangleFigure(10, 20, 10, 20, 90, 90, 90, 90));
    figures.push_back(new Square(20, 20, 20, 20, 90, 90, 90, 90));
    figures.push_back(new Parallelogram(20, 30, 20, 30, 30, 40, 30, 40));
    figures.push_back(new Rhombus(30, 30, 30, 30, 30, 40, 30, 40));

    for (const auto* fig : figures) {
        fig->print_info();
        std::cout << "-----------------------------------------\n";
    }

    for (auto* fig : figures) {
        delete fig;
    }

    return EXIT_SUCCESS;
};
