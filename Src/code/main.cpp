#include <cstdint>
#include <iostream> 
#include <locale> 
#include <string>
#include <vector>
#include <memory>
#include <functional>

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

    std::vector<const Figure*> figures;

    // «Затычка»: теперь мы передаем функцию-фабрику `std::function<Figure*()>`
    // Объект `new` будет создаваться НАПРЯМУЮ внутри блока try-catch!
    auto try_create = [&](std::function<Figure*()> factory) {
        try {
            // Создание происходит ЗДЕСЬ, поэтому любой throw будет пойман
            Figure* fig_ptr = factory(); 
            figures.push_back(fig_ptr);
        }
        catch (const figure_error& ex) {
            std::cout << "⚠️ Не удалось создать фигуру! Лог перехвачен в main:\n" << ex.what() << "\n";
            std::cout << "-----------------------------------------\n";
        }
        catch (const std::exception& ex) {
            std::cout << "Неизвестная ошибка: " << ex.what() << "\n";
            std::cout << "-----------------------------------------\n";
        }
    };

    std::cout << "=== Инициализация и проверка фигур ===\n\n";

    // Передаем создание через лямбда-выражения `[] { return new ...; }`
    try_create([] { return new Figure(); });

    // --- ТРЕУГОЛЬНИКИ ---
    try_create([] { return new Triangle(10, 20, 30, 50, 60, 70); });
    try_create([] { return new RightTriangle(10, 20, 30, 50, 60); }); // Свалится, но main поймает!
    try_create([] { return new IsoscelesTriangle(10, 20, 50, 60); }); // Свалится, но main поймает!
    try_create([] { return new EquilateralTriangle(30); });

    // --- ЧЕТЫРЕХУГОЛЬНИКИ ---
    try_create([] { return new Quadrangle(10, 20, 30, 40, 50, 60, 70, 80); }); // Свалится, но main поймает!
    try_create([] { return new Parallelogram(20, 30, 30, 40); });             // Свалится, но main поймает!
    try_create([] { return new RectangleFigure(10, 20); });
    try_create([] { return new Rhombus(30, 30, 40); });                       // Свалится, но main поймает!
    try_create([] { return new Square(20); });


    std::cout << "\n=== Вывод информации об успешных фигурах ===\n\n";

    for (const auto* fig : figures) {
        print_info(fig);
    }

    // Чистим память только за успешными фигурами
    for (const auto* fig : figures) {
        delete fig;
    }

    return EXIT_SUCCESS;
}