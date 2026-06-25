#pragma once 
#include <Figure/Base.hpp>


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


REGISTER_PARENT(Quadrangle, Figure)
  REGISTER_PARENT(Parallelogram, Quadrangle)
    REGISTER_PARENT(RectangleFigure, Parallelogram)
      REGISTER_PARENT(Square, RectangleFigure)
    REGISTER_PARENT(Rhombus, Parallelogram)