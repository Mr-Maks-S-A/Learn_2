#pragma once 
#include <Figure/Base.hpp>

class Triangle : public Figure {
public:
    Triangle(int a, int b, int c, int A, int B, int C, const std::string& name = "Треугольник") 
    try : Figure(name) {
        s = {a, b, c}; n = {A, B, C}; sides_count = 3;
        if (A + B + C != 180) {
            THROW_FIGURE_ERROR("сумма углов не равна 180");
        }
    }
    catch (const figure_error& ex) {
        HANDLE_CONSTRUCTOR_ERROR(ex);
    }
    void render_hierarchy() const override { Figure::render_hierarchy(); render_class_line<Triangle>("Triangle"); }
};

class RightTriangle : public Triangle {
public:
    RightTriangle(int a, int b, int c, int A, int B) 
    try : Triangle(a, b, c, A, B, 90, "Прямоугольный треугольник") {
        if (n.C != 90) { // На всякий случай, если базовый класс изменит логику
            THROW_FIGURE_ERROR("угол C не равен 90");
        }
    }
    catch (const figure_error& ex) {
        HANDLE_CONSTRUCTOR_ERROR(ex);
    }
    void render_hierarchy() const override { Triangle::render_hierarchy(); render_class_line<RightTriangle>("RightTriangle"); }
};

class IsoscelesTriangle : public Triangle {
public:
    IsoscelesTriangle(int a, int b, int A, int B) 
    try : Triangle(a, b, a, A, B, A, "Равнобедренный треугольник") {
        if (s.a != s.c || n.A != n.C) {
            THROW_FIGURE_ERROR("стороны a и c или углы A и C не равны");
        }
    }
    catch (const figure_error& ex) {
        HANDLE_CONSTRUCTOR_ERROR(ex);
    }
    void render_hierarchy() const override { Triangle::render_hierarchy(); render_class_line<IsoscelesTriangle>("IsoscelesTriangle"); }
};

class EquilateralTriangle : public Triangle {
public:
    EquilateralTriangle(int side) 
    try : Triangle(side, side, side, 60, 60, 60, "Равносторонний треугольник") {
        if (s.a != s.b || s.b != s.c || n.A != 60 || n.B != 60 || n.C != 60) {
            THROW_FIGURE_ERROR("не все стороны равны или углы не равны 60");
        }
    }
    catch (const figure_error& ex) {
        HANDLE_CONSTRUCTOR_ERROR(ex);
    }
    void render_hierarchy() const override { Triangle::render_hierarchy(); render_class_line<EquilateralTriangle>("EquilateralTriangle"); }
};



REGISTER_PARENT(Triangle, Figure)
  REGISTER_PARENT(RightTriangle, Triangle)
  REGISTER_PARENT(IsoscelesTriangle, Triangle)
  REGISTER_PARENT(EquilateralTriangle, Triangle)