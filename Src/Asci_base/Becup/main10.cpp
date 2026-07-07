// #include <iostream>
// #include <vector>
// #include <cmath>
// #include <algorithm>
// #include <fstream>
// #include <limits>
//
// // Разрешение текстового экрана
// const int WIDTH = 140;
// const int HEIGHT = 60;
// // Коррекция пропорций текстового символа (символы обычно вытянуты по вертикали)
// const float ASPECT_CORRECTION = 2.1f;
// const float M_PI_F = 3.14159265f;
//
// struct Vec3 {
//     float x, y, z;
//     Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
//     Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
//     Vec3 operator*(float f) const { return {x * f, y * f, z * f}; }
//     float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
//     Vec3 cross(const Vec3& v) const {
//         return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
//     }
//     Vec3 normalize() const {
//         float len = std::sqrt(x*x + y*y + z*z);
//         return len > 0 ? Vec3{x/len, y/len, z/len} : Vec3{0,0,0};
//     }
// };
//
// struct Triangle {
//     Vec3 v[3];
//     Vec3 normal;
// };
//
// // Генератор тора
// std::vector<Triangle> createTorus(float R, float r, int radialSegments, int tubularSegments) {
//     std::vector<Triangle> mesh;
//     std::vector<std::vector<Vec3>> vertices(radialSegments, std::vector<Vec3>(tubularSegments));
//
//     for (int i = 0; i < radialSegments; ++i) {
//         float u = (float)i / radialSegments * 2.0f * M_PI_F;
//         for (int j = 0; j < tubularSegments; ++j) {
//             float v = (float)j / tubularSegments * 2.0f * M_PI_F;
//             float x = (R + r * cos(v)) * cos(u);
//             float y = (R + r * cos(v)) * sin(u);
//             float z = r * sin(v);
//             vertices[i][j] = {x, y, z};
//         }
//     }
//
//     for (int i = 0; i < radialSegments; ++i) {
//         int next_i = (i + 1) % radialSegments;
//         for (int j = 0; j < tubularSegments; ++j) {
//             int next_j = (j + 1) % tubularSegments;
//
//             Triangle t1; t1.v[0] = vertices[i][j]; t1.v[1] = vertices[next_i][j]; t1.v[2] = vertices[i][next_j];
//             t1.normal = (t1.v[1] - t1.v[0]).cross(t1.v[2] - t1.v[0]).normalize();
//             mesh.push_back(t1);
//
//             Triangle t2; t2.v[0] = vertices[next_i][j]; t2.v[1] = vertices[next_i][next_j]; t2.v[2] = vertices[i][next_j];
//             t2.normal = (t2.v[1] - t2.v[0]).cross(t2.v[2] - t2.v[0]).normalize();
//             mesh.push_back(t2);
//         }
//     }
//     return mesh;
// }
//
// int main() {
//     // Текстовый буфер экрана
//     std::vector<char> screenBuffer(WIDTH * HEIGHT, ' ');
//     std::vector<Vec3> normalBuffer(WIDTH * HEIGHT, Vec3{0, 0, 0});
//     std::vector<float> zBuffer(WIDTH * HEIGHT, -std::numeric_limits<float>::max());
//
//     auto sceneMesh = createTorus(1.4f, 0.6f, 50, 25);
//
//     // Вращение (подобрано для Stone Story перспективы)
//     float angleX = 0.6f;
//     float angleY = 0.8f;
//     Vec3 cameraPos = {0, 0, -5.5f};
//
//     // Направление света для легкой тени (опционально, в стиле Stone Story)
//     Vec3 lightDir = Vec3{1.0f, 1.0f, -1.0f}.normalize();
//
//     // 1. РАСТЕРИЗАЦИЯ ГЕОМЕТРИИ
//     for (const auto& t : sceneMesh) {
//         Triangle transformed = t;
//         for (int i = 0; i < 3; ++i) {
//             // Ротация Y
//             float x1 = transformed.v[i].x * cos(angleY) - transformed.v[i].z * sin(angleY);
//             float z1 = transformed.v[i].x * sin(angleY) + transformed.v[i].z * cos(angleY);
//             // Ротация X
//             float y2 = transformed.v[i].y * cos(angleX) - z1 * sin(angleX);
//             float z2 = transformed.v[i].y * sin(angleX) + z1 * cos(angleX);
//
//             transformed.v[i] = {x1, y2, z2 + cameraPos.z};
//         }
//
//         Vec3 e1 = transformed.v[1] - transformed.v[0];
//         Vec3 e2 = transformed.v[2] - transformed.v[0];
//         Vec3 norm = e1.cross(e2).normalize();
//
//         // Проекция с учетом ASPECT_CORRECTION
//         int x[3], y[3];
//         float scaleY = 18.0f;
//         float scaleX = scaleY * ASPECT_CORRECTION;
//
//         for (int i = 0; i < 3; ++i) {
//             x[i] = static_cast<int>(transformed.v[i].x * scaleX + WIDTH / 2);
//             y[i] = static_cast<int>(transformed.v[i].y * scaleY + HEIGHT / 2);
//         }
//
//         int minX = std::max(0, std::min({x[0], x[1], x[2]}));
//         int maxX = std::min(WIDTH - 1, std::max({x[0], x[1], x[2]}));
//         int minY = std::max(0, std::min({y[0], y[1], y[2]}));
//         int maxY = std::min(HEIGHT - 1, std::max({y[0], y[1], y[2]}));
//
//         for (int py = minY; py <= maxY; ++py) {
//             for (int px = minX; px <= maxX; ++px) {
//                 float d = (y[1] - y[2]) * (x[0] - x[2]) + (x[2] - x[1]) * (y[0] - y[2]);
//                 if (std::abs(d) < 0.00001f) continue;
//
//                 float w0 = ((y[1] - y[2]) * (px - x[2]) + (x[2] - x[1]) * (py - y[2])) / d;
//                 float w1 = ((y[2] - y[0]) * (px - x[2]) + (x[0] - x[2]) * (py - y[2])) / d;
//                 float w2 = 1.0f - w0 - w1;
//
//                 if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
//                     float z = w0 * transformed.v[0].z + w1 * transformed.v[1].z + w2 * transformed.v[2].z;
//                     int idx = py * WIDTH + px;
//                     if (z > zBuffer[idx]) {
//                         zBuffer[idx] = z;
//                         normalBuffer[idx] = norm;
//                     }
//                 }
//             }
//         }
//     }
//
//     // 2. СТИЛИЗАЦИЯ И ПОИСК НАПРАВЛЕНИЯ СИМВОЛОВ (Stone Story ШАГ)
//     for (int y = 1; y < HEIGHT - 1; ++y) {
//         for (int x = 1; x < WIDTH - 1; ++x) {
//             int idx = y * WIDTH + x;
//
//             Vec3 n_up    = normalBuffer[(y - 1) * WIDTH + x];
//             Vec3 n_down  = normalBuffer[(y + 1) * WIDTH + x];
//             Vec3 n_left  = normalBuffer[y * WIDTH + (x - 1)];
//             Vec3 n_right = normalBuffer[y * WIDTH + (x + 1)];
//
//             // Считаем градиент изменений нормалей по X и Y
//             float dx = (n_right - n_left).dot(n_right - n_left);
//             float dy = (n_up - n_down).dot(n_up - n_down);
//             float edge = dx + dy;
//
//             if (edge > 0.15f) {
//                 // Вычисляем угол наклона ребра, чтобы выбрать правильный символ ASCII
//                 float angle = std::atan2(dy, dx) * 180.0f / M_PI_F;
//                 if (angle < 0) angle += 180.0f;
//
//                 // Маппинг углов в символы контура в зависимости от направления линии
//                 if (angle >= 0 && angle < 22.5)       screenBuffer[idx] = '|'; // Вертикальное ребро (изменение по X)
//                 else if (angle >= 22.5 && angle < 67.5)  screenBuffer[idx] = '/'; // Диагональ
//                 else if (angle >= 67.5 && angle < 112.5) screenBuffer[idx] = '-'; // Горизонтальное ребро
//                 else if (angle >= 112.5 && angle < 157.5) screenBuffer[idx] = '\\';// Обратная диагональ
//                 else                                     screenBuffer[idx] = '|';
//             } else {
//                 // Если это внутренняя часть геометрии (тело тора)
//                 Vec3 norm = normalBuffer[idx];
//                 if (norm.dot(norm) > 0.1f) {
//                     // Рассчитываем освещенность для легкого "текстурного" объема
//                     float intensity = norm.dot(lightDir);
//                     if (intensity > 0.6f)       screenBuffer[idx] = ' '; // Яркие места — чистые
//                     else if (intensity > 0.3f)  screenBuffer[idx] = '.'; // Легкая полутень
//                     else if (intensity > 0.0f)  screenBuffer[idx] = ':'; // Тень плотнее
//                     else                        screenBuffer[idx] = 'o'; // Глубокая тень внутри
//                 }
//             }
//         }
//     }
//
//     // 3. ВЫВОД РЕЗУЛЬТАТА И ЗАПИСЬ В ФАЙЛ
//     std::ofstream outFile("stone_story_output.txt");
//
//     for (int y = 0; y < HEIGHT; ++y) {
//         for (int x = 0; x < WIDTH; ++x) {
//             char c = screenBuffer[y * WIDTH + x];
//             std::cout << c;
//             if (outFile.is_open()) outFile << c;
//         }
//         std::cout << '\n';
//         if (outFile.is_open()) outFile << '\n';
//     }
//
//     if (outFile.is_open()) {
//         outFile.close();
//         std::cout << "\n[Успешно сохранено в файл: stone_story_output.txt]\n";
//     }
//
//     return 0;
// }














/*
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

const int WIDTH = 35;
const int HEIGHT = 15;
const float ASPECT_CORRECTION = 2.2f;
const float M_PI_F = 3.14159265f;

struct Vec3 {
    float x, y, z;
    Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vec3 operator*(float f) const { return {x * f, y * f, z * f}; }
    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 cross(const Vec3& v) const { return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x}; }
    Vec3 normalize() const { float len = std::sqrt(x*x + y*y + z*z); return len > 0 ? Vec3{x/len, y/len, z/len} : Vec3{0,0,0}; }
};

struct Edge {
    int v1, v2;
    int tri1, tri2 = -1; // Индексы прилегающих треугольников
};

struct Triangle {
    int vIdx[3]; // Индексы исходных вершин
    Vec3 normal;
};

// Рисование линии Брезенхэма в буфер с выбором правильного ASCII символа
void drawAsciiLine(std::vector<char>& screen, int x0, int y0, int x1, int y1, char forcedChar = 0) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    // Определяем символ линии по ее фактическому наклону на экране
    char edgeChar = forcedChar;
    if (edgeChar == 0) {
        if (dx > dy * 2) edgeChar = '-';
        else if (dy > dx * 2) edgeChar = '|';
        else if ((sx > 0 && sy > 0) || (sx < 0 && sy < 0)) edgeChar = '\\';
        else edgeChar = '/';
    }

    while (true) {
        if (x0 >= 0 && x0 < WIDTH && y0 >= 0 && y0 < HEIGHT) {
            screen[y0 * WIDTH + x0] = edgeChar; // Перезаписываем (приоритет линии)
        }
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx)  { err += dx; y0 += sy; }
    }
}

int main() {
    std::vector<char> screenBuffer(WIDTH * HEIGHT, ' ');
    std::vector<float> zBuffer(WIDTH * HEIGHT, -std::numeric_limits<float>::max());

    // Геометрия тора (низкополигональная, чтобы ребра ложились четко)
    const int RADIAL = 16;
    const int TUBULAR = 12;
    float R = 1.3f, r = 0.5f;

    std::vector<Vec3> localVertices;
    std::vector<Triangle> triangles;
    std::vector<Edge> edges;

    // Генерируем вершины без варнингов
    for (int i = 0; i < RADIAL; ++i) {
        float u = (float)i / RADIAL * 2.0f * M_PI_F;
        for (int j = 0; j < TUBULAR; ++j) {
            float v = (float)j / TUBULAR * 2.0f * M_PI_F;

            // Используем cosf и sinf вместо double-версий
            float x = (R + r * cosf(v)) * cosf(u);
            float y = (R + r * cosf(v)) * sinf(u);
            float z = r * sinf(v);

            localVertices.push_back({x, y, z});
        }
    }

    // Соединяем в треугольники и регистрируем ребра
    auto addEdge = [&](int v1, int v2, int triIdx) {
        if (v1 > v2) std::swap(v1, v2);
        for (auto& e : edges) {
            if (e.v1 == v1 && e.v2 == v2) { e.tri2 = triIdx; return; }
        }
        edges.push_back({v1, v2, triIdx, -1});
    };

    int triIdx = 0;
    for (int i = 0; i < RADIAL; ++i) {
        int next_i = (i + 1) % RADIAL;
        for (int j = 0; j < TUBULAR; ++j) {
            int next_j = (j + 1) % TUBULAR;
            int v0 = i * TUBULAR + j;
            int v1 = next_i * TUBULAR + j;
            int v2 = i * TUBULAR + next_j;
            int v3 = next_i * TUBULAR + next_j;

            triangles.push_back({{v0, v1, v2}, {}});
            addEdge(v0, v1, triIdx); addEdge(v1, v2, triIdx); addEdge(v2, v0, triIdx);
            triIdx++;

            triangles.push_back({{v1, v3, v2}, {}});
            addEdge(v1, v3, triIdx); addEdge(v3, v2, triIdx); addEdge(v2, v1, triIdx);
            triIdx++;
        }
    }

    // Трансформация (Поворот)
    float angleX = 0.5f, angleY = 0.6f;
    Vec3 cameraPos = {0, 0, -4.5f};
    std::vector<Vec3> projVertices(localVertices.size());
    std::vector<std::pair<int, int>> screenCoords(localVertices.size());

    float scaleY = 4.3f;
    float scaleX = scaleY * ASPECT_CORRECTION;

    for (size_t i = 0; i < localVertices.size(); ++i) {
        Vec3 v = localVertices[i];
        float x1 = v.x * cosf(angleY) - v.z * sinf(angleY);
        float z1 = v.x * sinf(angleY) + v.z * cosf(angleY);
        float y2 = v.y * cosf(angleX) - z1 * sinf(angleX);
        float z2 = v.y * sinf(angleX) + z1 * cosf(angleX);

        projVertices[i] = {x1, y2, z2 + cameraPos.z};
        screenCoords[i] = {
            static_cast<int>(x1 * scaleX + WIDTH / 2),
            static_cast<int>(y2 * scaleY + HEIGHT / 2)
        };
    }

    // Считаем нормали треугольников в пространстве камеры
    for (auto& t : triangles) {
        Vec3 e1 = projVertices[t.vIdx[1]] - projVertices[t.vIdx[0]];
        Vec3 e2 = projVertices[t.vIdx[2]] - projVertices[t.vIdx[0]];
        t.normal = e1.cross(e2).normalize();
    }

    // ШАГ 1: Заливка внутренности (Solid Body) точками через Z-буфер
    for (const auto& t : triangles) {
        // Backface culling (не рендерим то, что отвернуто от камеры)
        if (t.normal.z < 0) continue;

        int x0 = screenCoords[t.vIdx[0]].first,  y0 = screenCoords[t.vIdx[0]].second;
        int x1 = screenCoords[t.vIdx[1]].first,  y1 = screenCoords[t.vIdx[1]].second;
        int x2 = screenCoords[t.vIdx[2]].first,  y2 = screenCoords[t.vIdx[2]].second;

        int minX = std::max(0, std::min({x0, x1, x2}));
        int maxX = std::min(WIDTH - 1, std::max({x0, x1, x2}));
        int minY = std::max(0, std::min({y0, y1, y2}));
        int maxY = std::min(HEIGHT - 1, std::max({y0, y1, y2}));

        for (int py = minY; py <= maxY; ++py) {
            for (int px = minX; px <= maxX; ++px) {
                float d = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);
                if (std::abs(d) < 0.00001f) continue;
                float w0 = ((y1 - y2) * (px - x2) + (x2 - x1) * (py - y2)) / d;
                float w1 = ((y2 - y0) * (px - x2) + (x0 - x2) * (py - y2)) / d;
                float w2 = 1.0f - w0 - w1;

                if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                    float z = w0 * projVertices[t.vIdx[0]].z + w1 * projVertices[t.vIdx[1]].z + w2 * projVertices[t.vIdx[2]].z;
                    int idx = py * WIDTH + px;
                    if (z > zBuffer[idx]) {
                        zBuffer[idx] = z;
                        screenBuffer[idx] = '.'; // Чистая, аккуратная текстура внутренности
                    }
                }
            }
        }
    }

    // ШАГ 2: Рендеринг ART LINE поверх (Приоритетная отрисовка контуров)
    Vec3 viewDir = {0, 0, 1}; // Направление взгляда камеры
    for (const auto& e : edges) {
        bool isSilhouette = false;

        float z1 = triangles[e.tri1].normal.dot(viewDir);
        float z2 = (e.tri2 != -1) ? triangles[e.tri2].normal.dot(viewDir) : -1.0f;

        // Ребро является силуэтным, если одна грань смотрит на камеру, а вторая от нее (или отсутствует)
        if ((z1 >= 0 && z2 < 0) || (z1 < 0 && z2 >= 0)) {
            isSilhouette = true;
        }
        // Или если это внутреннее, но очень острое ребро стыка плоскостей
        else if (e.tri2 != -1) {
            float dotNormals = triangles[e.tri1].normal.dot(triangles[e.tri2].normal);
            if (dotNormals < 0.75f) { // Порог излома геометрии
                isSilhouette = true;
            }
        }

        if (isSilhouette) {
            int x0 = screenCoords[e.v1].first, y0 = screenCoords[e.v1].second;
            int x1 = screenCoords[e.v2].first, y1 = screenCoords[e.v2].second;
            // Рисуем чистую Брезенхэм-линию, она затирает фоновые точки '.'
            drawAsciiLine(screenBuffer, x0, y0, x1, y1);
        }
    }

    // Вывод в консоль
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            std::cout << screenBuffer[y * WIDTH + x];
        }
        std::cout << '\n';
    }

    return 0;
}*/


#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

const int WIDTH = 35;
const int HEIGHT = 15;
const float ASPECT_CORRECTION = 2.4f; // Чуть увеличил для лучшей читаемости формы
const float M_PI_F = 3.14159265f;

struct Vec3 {
    float x, y, z;
    Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vec3 operator*(float f) const { return {x * f, y * f, z * f}; }
    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 cross(const Vec3& v) const { return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x}; }
    Vec3 normalize() const { float len = std::sqrt(x*x + y*y + z*z); return len > 0 ? Vec3{x/len, y/len, z/len} : Vec3{0,0,0}; }
};

struct Edge {
    int v1, v2;
    int tri1, tri2 = -1;
};

struct Triangle {
    int vIdx[3];
    Vec3 normal;
};

void drawAsciiLine(std::vector<char>& screen, int x0, int y0, int x1, int y1, char forcedChar = 0) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    char edgeChar = forcedChar;
    if (edgeChar == 0) {
        if (dx > dy * 2) edgeChar = '-';
        else if (dy > dx * 2) edgeChar = '|';
        else if ((sx > 0 && sy > 0) || (sx < 0 && sy < 0)) edgeChar = '\\';
        else edgeChar = '/';
    }

    while (true) {
        if (x0 >= 0 && x0 < WIDTH && y0 >= 0 && y0 < HEIGHT) {
            screen[y0 * WIDTH + x0] = edgeChar;
        }
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx)  { err += dx; y0 += sy; }
    }
}

int main() {
    std::vector<char> screenBuffer(WIDTH * HEIGHT, ' ');
    std::vector<float> zBuffer(WIDTH * HEIGHT, -std::numeric_limits<float>::max());

    // 1. ВЕРШИННЫЙ МАССИВ СЮЗАННЫ (Упрощенный Low-Poly вариант)
    std::vector<Vec3> localVertices = {
        // Голова / Лоб
        {0.0f, 0.9f, 0.2f},    // 0: макушка
        {-0.5f, 0.7f, 0.4f},   // 1: левый лоб
        {0.5f, 0.7f, 0.4f},    // 2: правый лоб
        // Надбровные дуги / Брови
        {-0.6f, 0.3f, 0.7f},   // 3: левая бровь внешняя
        {-0.2f, 0.3f, 0.8f},   // 4: левая бровь внутренняя
        {0.2f, 0.3f, 0.8f},    // 5: правая бровь внутренняя
        {0.6f, 0.3f, 0.7f},    // 6: правая бровь внешняя
        // Глаза / Морда
        {-0.4f, 0.0f, 0.7f},   // 7: левый глаз
        {0.4f, 0.0f, 0.7f},    // 8: правый глаз
        {0.0f, -0.1f, 0.9f},   // 9: нос центр
        // Челюсть / Рот
        {-0.3f, -0.4f, 0.8f},  // 10: левая губа
        {0.3f, -0.4f, 0.8f},   // 11: правая губа
        {0.0f, -0.6f, 0.6f},   // 12: подбородок
        // Уши Левое
        {-0.9f, 0.4f, -0.1f},  // 13: верх левого уха
        {-1.2f, 0.1f, -0.2f},  // 14: край левого уха
        {-0.9f, -0.2f, -0.1f}, // 15: низ левого уха
        // Уши Правое
        {0.9f, 0.4f, -0.1f},   // 16: верх правого уха
        {1.2f, 0.1f, -0.2f},   // 17: край правого уха
        {0.9f, -0.2f, -0.1f},  // 18: низ правого уха
        // Затылок
        {0.0f, 0.5f, -0.8f},   // 19: верх затылка
        {0.0f, -0.4f, -0.7f}   // 20: основание черепа
    };

    // 2. ИНДЕКСНЫЙ МАССИВ ТРЕУГОЛЬНИКОВ СЮЗАННЫ
    std::vector<Triangle> triangles = {
        // Лоб и центр морды
        {{0, 1, 4}, {}}, {{0, 4, 5}, {}}, {{0, 5, 2}, {}},
        {{1, 3, 4}, {}}, {{2, 5, 6}, {}},
        // Нос и глаза
        {{4, 3, 7}, {}}, {{4, 7, 9}, {}}, {{5, 9, 8}, {}}, {{5, 8, 6}, {}},
        {{7, 10, 9}, {}}, {{8, 9, 11}, {}},
        // Подбородок / Челюсть
        {{10, 12, 9}, {}}, {{11, 9, 12}, {}},
        // Левое ухо
        {{1, 13, 3}, {}}, {{13, 14, 3}, {}}, {{3, 14, 15}, {}}, {{3, 15, 7}, {}}, {{7, 15, 10}, {}},
        // Правое ухо
        {{2, 6, 16}, {}}, {{16, 6, 17}, {}}, {{6, 17, 18}, {}}, {{6, 18, 8}, {}}, {{8, 18, 11}, {}},
        // Затылок и закрытие формы
        {{0, 19, 1}, {}}, {{0, 2, 19}, {}},
        {{13, 19, 14}, {}}, {{16, 17, 19}, {}},
        {{14, 20, 15}, {}}, {{17, 18, 20}, {}},
        {{19, 20, 14}, {}}, {{19, 17, 20}, {}},
        {{15, 20, 12}, {}}, {{18, 12, 20}, {}},
        {{10, 15, 12}, {}}, {{11, 12, 18}, {}}
    };

    // Автоматическая генерация уникальных ребер на основе треугольников
    std::vector<Edge> edges;
    auto addEdge = [&](int v1, int v2, int triIdx) {
        if (v1 > v2) std::swap(v1, v2);
        for (auto& e : edges) {
            if (e.v1 == v1 && e.v2 == v2) { e.tri2 = triIdx; return; }
        }
        edges.push_back({v1, v2, triIdx, -1});
    };

    for (size_t i = 0; i < triangles.size(); ++i) {
        addEdge(triangles[i].vIdx[0], triangles[i].vIdx[1], i);
        addEdge(triangles[i].vIdx[1], triangles[i].vIdx[2], i);
        addEdge(triangles[i].vIdx[2], triangles[i].vIdx[0], i);
    }

    // Вращение: Сюзанна смотрит чуть в сторону и наклонена (классический ракурс)
    float angleX = 0.2f;
    float angleY = 0.5f; // Поворот три четверти
    Vec3 cameraPos = {0, 0, -3.2f};

    std::vector<Vec3> projVertices(localVertices.size());
    std::vector<std::pair<int, int>> screenCoords(localVertices.size());

    float scaleY = 5.5f; // Чуть увеличили масштаб под форму головы
    float scaleX = scaleY * ASPECT_CORRECTION;

    for (size_t i = 0; i < localVertices.size(); ++i) {
        Vec3 v = localVertices[i];
        float x1 = v.x * cosf(angleY) - v.z * sinf(angleY);
        float z1 = v.x * sinf(angleY) + v.z * cosf(angleY);
        float y2 = v.y * cosf(angleX) - z1 * sinf(angleX);
        float z2 = v.y * sinf(angleX) + z1 * cosf(angleX);

        projVertices[i] = {x1, y2, z2 + cameraPos.z};
        screenCoords[i] = {
            static_cast<int>(x1 * scaleX + WIDTH / 2),
            static_cast<int>(y2 * scaleY + HEIGHT / 2)
        };
    }

    for (auto& t : triangles) {
        Vec3 e1 = projVertices[t.vIdx[1]] - projVertices[t.vIdx[0]];
        Vec3 e2 = projVertices[t.vIdx[2]] - projVertices[t.vIdx[0]];
        t.normal = e1.cross(e2).normalize();
    }

    // ШАГ 1: Тело (заливка внутренностей точками)
    for (const auto& t : triangles) {
        if (t.normal.z < 0) continue;

        int x0 = screenCoords[t.vIdx[0]].first,  y0 = screenCoords[t.vIdx[0]].second;
        int x1 = screenCoords[t.vIdx[1]].first,  y1 = screenCoords[t.vIdx[1]].second;
        int x2 = screenCoords[t.vIdx[2]].first,  y2 = screenCoords[t.vIdx[2]].second;

        int minX = std::max(0, std::min({x0, x1, x2}));
        int maxX = std::min(WIDTH - 1, std::max({x0, x1, x2}));
        int minY = std::max(0, std::min({y0, y1, y2}));
        int maxY = std::min(HEIGHT - 1, std::max({y0, y1, y2}));

        for (int py = minY; py <= maxY; ++py) {
            for (int px = minX; px <= maxX; ++px) {
                float d = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);
                if (std::abs(d) < 0.00001f) continue;
                float w0 = ((y1 - y2) * (px - x2) + (x2 - x1) * (py - y2)) / d;
                float w1 = ((y2 - y0) * (px - x2) + (x0 - x2) * (py - y2)) / d;
                float w2 = 1.0f - w0 - w1;

                if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                    float z = w0 * projVertices[t.vIdx[0]].z + w1 * projVertices[t.vIdx[1]].z + w2 * projVertices[t.vIdx[2]].z;
                    int idx = py * WIDTH + px;
                    if (z > zBuffer[idx]) {
                        zBuffer[idx] = z;
                        screenBuffer[idx] = '.';
                    }
                }
            }
        }
    }

    // ШАГ 2: Чистый приоритетный Art Line
    Vec3 viewDir = {0, 0, 1};
    for (const auto& e : edges) {
        bool isLineArt = false;

        float z1 = triangles[e.tri1].normal.dot(viewDir);
        float z2 = (e.tri2 != -1) ? triangles[e.tri2].normal.dot(viewDir) : -1.0f;

        // Внешний силуэт (граница с пустотой)
        if ((z1 >= 0 && z2 < 0) || (z1 < 0 && z2 >= 0)) {
            isLineArt = true;
        }
        // Внутренние важные складки мордочки (стык бровей, носа, ушей)
        else if (e.tri2 != -1) {
            float dotNormals = triangles[e.tri1].normal.dot(triangles[e.tri2].normal);
            if (dotNormals < 0.65f) { // Острые стыки полигонов
                isLineArt = true;
            }
        }

        if (isLineArt) {
            int x0 = screenCoords[e.v1].first, y0 = screenCoords[e.v1].second;
            int x1 = screenCoords[e.v2].first, y1 = screenCoords[e.v2].second;
            drawAsciiLine(screenBuffer, x0, y0, x1, y1);
        }
    }

    // Вывод кадра
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            std::cout << screenBuffer[y * WIDTH + x];
        }
        std::cout << '\n';
    }

    return 0;
}
