#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>

// Простая структура для 3D вектора
struct Vector3 {
    float x, y, z;
};

// Ребро, соединяющее два индекса вершин
struct Edge {
    int a, b;
};

// Размеры экрана (символьного буфера)
const int WIDTH = 80;
const int HEIGHT = 40;
const float FOV = 60.0f; // Угол обзора

// Алгоритм Брезенхэма для рисования линий в ASCII-буфере
void drawLine(int x0, int y0, int x1, int y1, char ch, std::vector<char>& buffer) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (x0 >= 0 && x0 < WIDTH && y0 >= 0 && y0 < HEIGHT) {
            buffer[y0 * WIDTH + x0] = ch;
        }

        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

int main() {
    // 1. Геометрия куба (8 вершин, 12 ребер)
    std::vector<Vector3> vertices = {
        {-1, -1, -1}, { 1, -1, -1}, { 1,  1, -1}, {-1,  1, -1},
        {-1, -1,  1}, { 1, -1,  1}, { 1,  1,  1}, {-1,  1,  1}
    };

    std::vector<Edge> edges = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Задняя грань
        {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Передняя грань
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Боковые ребра
    };

    float angleX = 0.0f;
    float angleY = 0.0f;

    // Очистка экрана (Escape-последовательность)
    std::cout << "\x1b[2J";

    while (true) {
        // Создаем и очищаем буфер кадра (заполняем пробелами)
        std::vector<char> buffer(WIDTH * HEIGHT, ' ');

        // Вращение и проекция вершин
        std::vector<std::pair<int, int>> projectedVertices;

        float radX = angleX * 3.14159265f / 180.0f;
        float radY = angleY * 3.14159265f / 180.0f;

        for (const auto& v : vertices) {
            // Вращение вокруг оси Y
            float x1 = v.x * std::cos(radY) - v.z * std::sin(radY);
            float z1 = v.x * std::sin(radY) + v.z * std::cos(radY);

            // Вращение вокруг оси X
            float y2 = v.y * std::cos(radX) - z1 * std::sin(radX);
            float z2 = v.y * std::sin(radX) + z1 * std::cos(radX);

            // Отодвигаем объект от камеры по оси Z
            float distance = 3.5f;
            float targetZ = z2 + distance;

            // Перспективная проекция
            // Множитель 2.0 для компенсации того, что символы в терминале выше, чем шире
            float screenX = (x1 / targetZ) * (WIDTH / 2.0f) + (WIDTH / 2.0f);
            float screenY = (y2 / targetZ) * (HEIGHT * 1.0f) + (HEIGHT / 2.0f);

            projectedVertices.push_back({static_cast<int>(screenX), static_cast<int>(screenY)});
        }

        // 2. Рендеринг линий (Line Art шейдинг)
        // Вместо заполнения пикселей, мы "чертим" контуры
        for (const auto& edge : edges) {
            auto p1 = projectedVertices[edge.a];
            auto p2 = projectedVertices[edge.b];

            // Используем '#' или '@' для четких, выразительных линий контура
            // drawLine(p1.x, p1.y, p2.x, p2.y, '#', buffer);
            drawLine(p1.first, p1.second, p2.first, p2.second, '#', buffer);
        }

        // 3. Вывод буфера на экран (Double Buffering эмуляция)
        std::cout << "\x1b[H"; // Переместить курсор в левый верхний угол
        std::string output = "";
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                output += buffer[y * WIDTH + x];
            }
            output += '\n';
        }
        std::cout << output << std::flush;

        // Изменяем углы для анимации
        angleX += 2.0f;
        angleY += 3.0f;

        std::this_thread::sleep_for(std::chrono::milliseconds(33)); // ~30 FPS
    }

    return 0;
}
