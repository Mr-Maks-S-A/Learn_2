/**
 * @file main.cpp
 * @brief Главный файл программы для демонстрации полиморфной иерархии геометрических фигур.
 * Выводит информацию о сторонах, углах и внутренней структуре наследования 
 * для треугольников и четырёхугольников.
 */

#include <cstdint>
#include <iostream> 
#include <locale> 
#include <string>

#include <Figure/BranchOfQuadrancles.hpp>
#include <Figure/BranchOfTriangles.hpp>

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
};