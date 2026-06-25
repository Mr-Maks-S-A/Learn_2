#pragma once
#include <Until/MyRedererOOP.hpp> 


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