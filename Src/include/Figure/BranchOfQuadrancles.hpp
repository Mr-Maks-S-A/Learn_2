#pragma once 
#include <Figure/Base.hpp>

class Quadrangle : public Figure {
public:
    Quadrangle(int a, int b, int c, int d, int A, int B, int C, int D, const std::string& name = "Четырёхугольник") 
    try : Figure(name) {
        s = {a, b, c, d}; n = {A, B, C, D}; sides_count = 4; is_quad = true;
        if (A + B + C + D != 360) {
           THROW_FIGURE_ERROR("сумма углов не равна 360");
        }
    } 
    catch (const figure_error& ex) {
        HANDLE_CONSTRUCTOR_ERROR(ex);
    } 

    void render_hierarchy() const override { Figure::render_hierarchy(); render_class_line<Quadrangle>("Quadrangle"); }
};

class Parallelogram : public Quadrangle {
public:
    Parallelogram(int a, int b, int A, int B, const std::string& name = "Параллелограмм") 
    try : Quadrangle(a, b, a, b, A, B, A, B, name) {
        if (s.a != s.c || s.b != s.d || n.A != n.C || n.B != n.D) {
           THROW_FIGURE_ERROR("противоположные стороны или углы не равны");
        }
    }
    catch (const figure_error& ex) {
        HANDLE_CONSTRUCTOR_ERROR(ex);
    }

    void render_hierarchy() const override { Quadrangle::render_hierarchy(); render_class_line<Parallelogram>("Parallelogram"); }
};

class RectangleFigure : public Parallelogram {
public:
    RectangleFigure(int a, int b, const std::string& name = "Прямоугольник") 
    try : Parallelogram(a, b, 90, 90, name) {
        if (n.A != 90 || n.B != 90 || n.C != 90 || n.D != 90) {
           THROW_FIGURE_ERROR("углы не равны 90");
        }
    }
    catch (const figure_error& ex) {
        HANDLE_CONSTRUCTOR_ERROR(ex);
    }

    void render_hierarchy() const override { Parallelogram::render_hierarchy(); render_class_line<RectangleFigure>("RectangleFigure"); }
};

class Rhombus : public Parallelogram {
public:
    Rhombus(int a, int A, int B, const std::string& name = "Ромб") 
    try : Parallelogram(a, a, A, B, name) {
        if (s.a != s.b || s.b != s.c || s.c != s.d) {
           THROW_FIGURE_ERROR("не все стороны равны");
        }
    }
    catch (const figure_error& ex) {
        HANDLE_CONSTRUCTOR_ERROR(ex);
    }

    void render_hierarchy() const override { Parallelogram::render_hierarchy(); render_class_line<Rhombus>("Rhombus"); }
};

class Square : public RectangleFigure {
public:
    Square(int a) 
    try : RectangleFigure(a, a, "Квадрат") {
        if (s.a != s.b || s.b != s.c || s.c != s.d) {
           THROW_FIGURE_ERROR("не все стороны квадрата равны");
        }
    }
    catch (const figure_error& ex) {
        HANDLE_CONSTRUCTOR_ERROR(ex);
    }

    void render_hierarchy() const override { RectangleFigure::render_hierarchy(); render_class_line<Square>("Square"); }
};

REGISTER_PARENT(Quadrangle, Figure)
  REGISTER_PARENT(Parallelogram, Quadrangle)
    REGISTER_PARENT(RectangleFigure, Parallelogram)
      REGISTER_PARENT(Square, RectangleFigure)
    REGISTER_PARENT(Rhombus, Parallelogram)