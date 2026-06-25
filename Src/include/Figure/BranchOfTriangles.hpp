#pragma once 
#include <Figure/Base.hpp>

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



REGISTER_PARENT(Triangle, Figure)
  REGISTER_PARENT(RightTriangle, Triangle)
  REGISTER_PARENT(IsoscelesTriangle, Triangle)
  REGISTER_PARENT(EquilateralTriangle, Triangle)