#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
#include <algorithm>

struct Vector3 {
    float x, y, z;

    // Вспомогательные методы для работы с векторами
    Vector3 operator-(const Vector3& o) const { return {x - o.x, y - o.y, z - o.z}; }
    Vector3 operator+(const Vector3& o) const { return {x + o.x, y + o.y, z + o.z}; }
    float dot(const Vector3& o) const { return x * o.x + y * o.y + z * o.z; }
    Vector3 cross(const Vector3& o) const {
        return { y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x };
    }
    void normalize() {
        float len = std::sqrt(x*x + y*y + z*z);
        if (len > 0) { x /= len; y /= len; z /= len; }
    }
};

struct Triangle {
    int v0, v1, v2;
};

const int WIDTH = 80;
const int HEIGHT = 40;

// Отрисовка линии Брезенхэма с проверкой Z-буфера
void drawLineZ(int x0, int y0, float z0, int x1, int y1, float z1,
               char ch, std::vector<char>& buffer, std::vector<float>& zBuffer) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    int steps = std::max(dx, dy);
    int step = 0;

    while (true) {
        if (x0 >= 0 && x0 < WIDTH && y0 >= 0 && y0 < HEIGHT) {
            // Интерполяция глубины Z вдоль линии
            float t = (steps == 0) ? 1.0f : (float)step / steps;
            float currentZ = z0 + (z1 - z0) * t;

            int idx = y0 * WIDTH + x0;
            // Маленький offset (0.01), чтобы линии не "тонули" в собственных полигонах
            if (currentZ <= zBuffer[idx] + 0.01f) {
                buffer[idx] = ch;
            }
        }

        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
        step++;
    }
               }

               // Заполнение Z-буфера треугольниками (стандартная растеризация без вывода цвета)
               void rasterizeZBuffer(const Vector3& p0, const Vector3& p1, const Vector3& p2, std::vector<float>& zBuffer) {
                   int minX = std::max(0, static_cast<int>(std::min({p0.x, p1.x, p2.x})));
                   int maxX = std::min(WIDTH - 1, static_cast<int>(std::max({p0.x, p1.x, p2.x})));
                   int minY = std::max(0, static_cast<int>(std::min({p0.y, p1.y, p2.y})));
                   int maxY = std::min(HEIGHT - 1, static_cast<int>(std::max({p0.y, p1.y, p2.y})));

                   float denom = ((p1.y - p2.y) * (p0.x - p2.x) + (p2.x - p1.x) * (p0.y - p2.y));
                   if (std::abs(denom) < 0.00001f) return;

                   for (int y = minY; y <= maxY; ++y) {
                       for (int x = minX; x <= maxX; ++x) {
                           float w0 = ((p1.y - p2.y) * (x - p2.x) + (p2.x - p1.x) * (y - p2.y)) / denom;
                           float w1 = ((p2.y - p0.y) * (x - p2.x) + (p0.x - p2.x) * (y - p2.y)) / denom;
                           float w2 = 1.0f - w0 - w1;

                           if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                               float interpolatedZ = w0 * p0.z + w1 * p1.z + w2 * p2.z;
                               int idx = y * WIDTH + x;
                               if (interpolatedZ < zBuffer[idx]) {
                                   zBuffer[idx] = interpolatedZ;
                               }
                           }
                       }
                   }
               }

               int main() {
                   // Геометрия куба, разбитая на треугольники (12 штук)
                   std::vector<Vector3> vertices = {
                       {-1,-1,-1}, { 1,-1,-1}, { 1, 1,-1}, {-1, 1,-1},
                       {-1,-1, 1}, { 1,-1, 1}, { 1, 1, 1}, {-1, 1, 1}
                   };

                   std::vector<Triangle> triangles = {
                       {0,1,2}, {0,2,3}, {4,6,5}, {4,7,6}, // Задняя / Передняя
                       {0,4,1}, {1,4,5}, {1,5,2}, {2,5,6}, // Нижняя / Правая
                       {2,6,3}, {3,6,7}, {0,3,4}, {4,3,7}  // Верхняя / Левая
                   };

                   float angleX = 30.0f, angleY = 45.0f;
                   std::cout << "\x1b[2J";

                   while (true) {
                       std::vector<char> buffer(WIDTH * HEIGHT, ' ');
                       std::vector<float> zBuffer(WIDTH * HEIGHT, 10000.0f); // Очищаем Z-буфер "бесконечностью"

                       std::vector<Vector3> transformedVertices;
                       std::vector<Vector3> projectedVertices;

                       float radX = angleX * 3.14159265f / 180.0f;
                       float radY = angleY * 3.14159265f / 180.0f;

                       // Шаг 1: Трансформация сцены
                       for (const auto& v : vertices) {
                           // Вращение Y
                           float x1 = v.x * std::cos(radY) - v.z * std::sin(radY);
                           float z1 = v.x * std::sin(radY) + v.z * std::cos(radY);
                           // Вращение X
                           float y2 = v.y * std::cos(radX) - z1 * std::sin(radX);
                           float z2 = v.y * std::sin(radX) + z1 * std::cos(radX);

                           float distance = 3.5f;
                           Vector3 worldPos = { x1, y2, z2 + distance };
                           transformedVertices.push_back(worldPos);

                           // Проекция на экран
                           float scrX = (worldPos.x / worldPos.z) * (WIDTH / 2.0f) + (WIDTH / 2.0f);
                           float scrY = (worldPos.y / worldPos.z) * (HEIGHT * 1.0f) + (HEIGHT / 2.0f);
                           projectedVertices.push_back({scrX, scrY, worldPos.z});
                       }

                       // Шаг 2: Первичный проход — заполнение Z-буфера геометрией
                       for (const auto& t : triangles) {
                           rasterizeZBuffer(projectedVertices[t.v0], projectedVertices[t.v1], projectedVertices[t.v2], zBuffer);
                       }

                       // Шаг 3: Вторичный проход — шейдинг линий (Line Art)
                       for (const auto& t : triangles) {
                           Vector3 v0 = transformedVertices[t.v0];
                           Vector3 v1 = transformedVertices[t.v1];
                           Vector3 v2 = transformedVertices[t.v2];

                           // Считаем нормаль треугольника в мировых координатах
                           Vector3 edge1 = v1 - v0;
                           Vector3 edge2 = v2 - v0;
                           Vector3 normal = edge1.cross(edge2);
                           normal.normalize();

                           // Вектор направления взгляда камеры (из начала координат на объект)
                           Vector3 viewDir = { v0.x, v0.y, v0.z };
                           viewDir.normalize();

                           float dot = normal.dot(viewDir);

                           // Back-face culling: если полигон смотрит от нас, не рисуем его контур
                           if (dot < 0.0f) {
                               auto p0 = projectedVertices[t.v0];
                               auto p1 = projectedVertices[t.v1];
                               auto p2 = projectedVertices[t.v2];

                               // Настройка рельефного рисунка:
                               // Чем сильнее грань повернута к свету или «на излом» к камере, тем изящнее штрих.
                               // Для простого Line Art используем разные символы в зависимости от угла наклона полигона
                               char edgeChar = '#';
                               if (std::abs(dot) < 0.3f) edgeChar = '%'; // Силуэтная/касательная линия (ярче)
                               else if (std::abs(dot) < 0.7f) edgeChar = ':'; // Внутренний рельеф граней

                               // Рисуем 3 ребра треугольника, проходя тест глубины
                               drawLineZ(p0.x, p0.y, p0.z, p1.x, p1.y, p1.z, edgeChar, buffer, zBuffer);
                               drawLineZ(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, edgeChar, buffer, zBuffer);
                               drawLineZ(p2.x, p2.y, p2.z, p0.x, p0.y, p0.z, edgeChar, buffer, zBuffer);
                           }
                       }

                       // Вывод кадра
                       std::cout << "\x1b[H";
                       std::string output = "";
                       for (int y = 0; y < HEIGHT; ++y) {
                           for (int x = 0; x < WIDTH; ++x) {
                               output += buffer[y * WIDTH + x];
                           }
                           output += '\n';
                       }
                       std::cout << output << std::flush;

                       angleX += 1.0f;
                       angleY += 1.5f;
                       std::this_thread::sleep_for(std::chrono::milliseconds(33));
                   }
                   return 0;
               }
