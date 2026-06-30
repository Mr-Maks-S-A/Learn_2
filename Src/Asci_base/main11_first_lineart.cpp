// #include <iostream>
// #include <vector>
// #include <cmath>
// #include <algorithm>
// #include <cstdint>
// #include <fstream>
// #include <sstream>
// #include <string>
//
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"
//
// const int WIDTH = 1000;
// const int HEIGHT = 1000;
// const float M_PI_F = 3.1415926535f;
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
// // Функция загрузки ЛЮБОЙ триангулированной .obj модели
// std::vector<Triangle> loadOBJ(const std::string& filename) {
//     std::vector<Triangle> mesh;
//     std::vector<Vec3> vertices;
//
//     std::ifstream file(filename);
//     if (!file.is_open()) {
//         std::cerr << "Ошибка: Не удалось открыть файл " << filename << std::endl;
//         std::cerr << "Пожалуйста, экспортируйте Сюзанну из Blender в " << filename << std::endl;
//         return mesh;
//     }
//
//     std::string line;
//     while (std::getline(file, line)) {
//         std::istringstream ss(line);
//         std::string type;
//         ss >> type;
//
//         if (type == "v") { // Вершина
//             float x, y, z;
//             ss >> x >> y >> z;
//             vertices.push_back({x, y, z});
//         }
//         else if (type == "f") { // Грань (Полигон)
//             std::string v1_str, v2_str, v3_str;
//             ss >> v1_str >> v2_str >> v3_str;
//
//             // OBJ поддерживает форматы v, v/vt, v/vt/vn. Нам нужен только первый индекс (вершина)
//             auto get_v_idx = [](const std::string& s) {
//                 return std::stoi(s.substr(0, s.find('/'))) - 1; // Индексы в OBJ начинаются с 1
//             };
//
//             int idx0 = get_v_idx(v1_str);
//             int idx1 = get_v_idx(v2_str);
//             int idx2 = get_v_idx(v3_str);
//
//             // Проверка на корректность индексов
//             if (idx0 < vertices.size() && idx1 < vertices.size() && idx2 < vertices.size()) {
//                 Triangle t;
//                 t.v[0] = vertices[idx0];
//                 t.v[1] = vertices[idx1];
//                 t.v[2] = vertices[idx2];
//                 t.normal = (t.v[1] - t.v[0]).cross(t.v[2] - t.v[0]).normalize();
//                 mesh.push_back(t);
//             }
//         }
//     }
//     file.close();
//     std::cout << "Успешно загружено полигонов из OBJ: " << mesh.size() << std::endl;
//     return mesh;
// }
//
// int main() {
//     std::vector<Vec3> normalBuffer(WIDTH * HEIGHT, Vec3{0, 0, 0});
//     std::vector<float> zBuffer(WIDTH * HEIGHT, -std::numeric_limits<float>::max());
//     std::vector<uint8_t> image(WIDTH * HEIGHT * 4, 255); // Белый фон
//
//     // Загружаем полноразмерную Сюзанну из внешнего файла
//     std::vector<Triangle> sceneMesh = loadOBJ("suzanne.obj");
//
//     if (sceneMesh.empty()) {
//         std::cout << "Создаем пустую сцену, так как файл модели отсутствует." << std::endl;
//         return 1;
//     }
//
//     // Ракурс камеры (Сюзанна в три четверти)
//     float angleX = 0.3f;
//     float angleY = 0.6f;
//     Vec3 cameraPos = {0, 0, -4.5f};
//
//     // 1. РЕНДЕРИНГ ГЕОМЕТРИИ
//     for (const auto& t : sceneMesh) {
//         Triangle transformed = t;
//
//         for (int i = 0; i < 3; ++i) {
//             float x1 = transformed.v[i].x * cosf(angleY) - transformed.v[i].z * sinf(angleY);
//             float z1 = transformed.v[i].x * sinf(angleY) + transformed.v[i].z * cosf(angleY);
//
//             float y2 = transformed.v[i].y * cosf(angleX) - z1 * sinf(angleX);
//             float z2 = transformed.v[i].y * sinf(angleX) + z1 * cosf(angleX);
//
//             transformed.v[i] = {x1, y2, z2 + cameraPos.z};
//         }
//
//         Vec3 e1 = transformed.v[1] - transformed.v[0];
//         Vec3 e2 = transformed.v[2] - transformed.v[0];
//         Vec3 norm = e1.cross(e2).normalize();
//
//         int x[3], y[3];
//         float scale = 300.0f; // Масштаб отображения на экране 1000х1000
//         for (int i = 0; i < 3; ++i) {
//             x[i] = static_cast<int>(transformed.v[i].x * scale + WIDTH / 2);
//             y[i] = static_cast<int>(transformed.v[i].y * scale + HEIGHT / 2);
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
//     // 2. ПОСТ-ОБРАБОТКА (LINE ART ФИЛЬТР СОБЕЛЯ)
//     for (int y = 1; y < HEIGHT - 1; ++y) {
//         for (int x = 1; x < WIDTH - 1; ++x) {
//             Vec3 n_up    = normalBuffer[(y - 1) * WIDTH + x];
//             Vec3 n_down  = normalBuffer[(y + 1) * WIDTH + x];
//             Vec3 n_left  = normalBuffer[y * WIDTH + (x - 1)];
//             Vec3 n_right = normalBuffer[y * WIDTH + (x + 1)];
//
//             float edgeX = (n_right - n_left).dot(n_right - n_left);
//             float edgeY = (n_up - n_down).dot(n_up - n_down);
//             float edge = edgeX + edgeY;
//
//             int imgIdx = (y * WIDTH + x) * 4;
//
//             // Для оригинальной Сюзанны шаг нормалей плотнее. Порог 0.08f-0.12f
//             // идеально прорисует все мелкие детали её глаз, ушей и надбровных дуг.
//             if (edge > 0.09f) {
//                 image[imgIdx + 0] = 0;
//                 image[imgIdx + 1] = 0;
//                 image[imgIdx + 2] = 0;
//             } else {
//                 Vec3 currentNormal = normalBuffer[y * WIDTH + x];
//                 if (currentNormal.dot(currentNormal) > 0.1f) {
//                     image[imgIdx + 0] = 238;
//                     image[imgIdx + 1] = 238;
//                     image[imgIdx + 2] = 238;
//                 }
//             }
//         }
//     }
//
//     stbi_write_png("full_suzanne_line_art.png", WIDTH, HEIGHT, 4, image.data(), WIDTH * 4);
//     std::cout << "Рендер оригинальной Сюзанны сохранен в full_suzanne_line_art.png!" << std::endl;
//
//     return 0;
// }















// #include <iostream>
// #include <vector>
// #include <cmath>
// #include <algorithm>
// #include <cstdint>
// #include <fstream>
// #include <sstream>
// #include <string>
//
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"
//
// const int WIDTH = 1000;
// const int HEIGHT = 1000;
// const float M_PI_F = 3.1415926535f;
//
// struct Vec3 {
//     float x, y, z;
//     Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
//     Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
//     Vec3 operator*(float f) const { return {x * f, y * f, z * f}; }
//     float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
//     Vec3 cross(const Vec3& v) const { return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x}; }
//     Vec3 normalize() const { float len = std::sqrt(x*x + y*y + z*z); return len > 0 ? Vec3{x/len, y/len, z/len} : Vec3{0,0,0}; }
// };
//
// struct Triangle {
//     int vIdx[3];
//     Vec3 normal; // Мировая/камерная нормаль
// };
//
// struct Edge {
//     int v1, v2;
//     int tri1 = -1;
//     int tri2 = -1;
// };
//
// // Функция пересечения луча с треугольником (Алгоритм Моллера-Трумбора)
// // Используется для векторной проверки видимости ребра
// bool rayTriangleIntersect(const Vec3& rayOrigin, const Vec3& rayDir, const Vec3& v0, const Vec3& v1, const Vec3& v2, float& t) {
//     Vec3 edge1 = v1 - v0;
//     Vec3 edge2 = v2 - v0;
//     Vec3 pvec = rayDir.cross(edge2);
//     float det = edge1.dot(pvec);
//
//     if (det > -0.00001f && det < 0.00001f) return false;
//     float invDet = 1.0f / det;
//
//     Vec3 tvec = rayOrigin - v0;
//     float u = tvec.dot(pvec) * invDet;
//     if (u < 0.0f || u > 1.0f) return false;
//
//     Vec3 qvec = tvec.cross(edge1);
//     float v = rayDir.dot(qvec) * invDet;
//     if (v < 0.0f || u + v > 1.0f) return false;
//
//     t = edge2.dot(qvec) * invDet;
//     return t > 0.00001f; // Пересечение истинно, если оно впереди луча
// }
//
// // Рисование гладкой векторной линии (Брезенхем с толщиной)
// void drawLine(std::vector<uint8_t>& image, int x0, int y0, int x1, int y1, int thickness) {
//     int dx = std::abs(x1 - x0);
//     int dy = std::abs(y1 - y0);
//     int sx = (x0 < x1) ? 1 : -1;
//     int sy = (y0 < y1) ? 1 : -1;
//     int err = dx - dy;
//
//     while (true) {
//         for (int ty = -thickness / 2; ty <= thickness / 2; ++ty) {
//             for (int tx = -thickness / 2; tx <= thickness / 2; ++tx) {
//                 int nx = x0 + tx;
//                 int ny = y0 + ty;
//                 if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
//                     int imgIdx = (ny * WIDTH + nx) * 4;
//                     image[imgIdx + 0] = 0; // Полностью черный цвет штриха
//                     image[imgIdx + 1] = 0;
//                     image[imgIdx + 2] = 0;
//                 }
//             }
//         }
//         if (x0 == x1 && y0 == y1) break;
//         int e2 = 2 * err;
//         if (e2 > -dy) { err -= dy; x0 += sx; }
//         if (e2 < dx)  { err += dx; y0 += sy; }
//     }
// }
//
// std::vector<Triangle> loadOBJ(const std::string& filename, std::vector<Vec3>& vertices) {
//     std::vector<Triangle> triangles;
//     std::ifstream file(filename);
//     if (!file.is_open()) return triangles;
//
//     std::string line;
//     while (std::getline(file, line)) {
//         std::istringstream ss(line);
//         std::string type;
//         ss >> type;
//         if (type == "v") {
//             float x, y, z;
//             ss >> x >> y >> z;
//             vertices.push_back({x, y, z});
//         } else if (type == "f") {
//             std::string v1, v2, v3;
//             ss >> v1 >> v2 >> v3;
//             auto get_idx = [](const std::string& s) { return std::stoi(s.substr(0, s.find('/'))) - 1; };
//             triangles.push_back({{get_idx(v1), get_idx(v2), get_idx(v3)}, {}});
//         }
//     }
//     return triangles;
// }
//
// int main() {
//     std::vector<Vec3> vertices;
//     std::vector<Triangle> triangles = loadOBJ("suzanne.obj", vertices);
//
//     if (triangles.empty()) {
//         std::cerr << "Положите файл suzanne.obj рядом с бинарником!" << std::endl;
//         return 1;
//     }
//
//     // Построение карты уникальных ребер (Mesh Topology)
//     std::vector<Edge> edges;
//     auto addEdge = [&](int v1, int v2, int triIdx) {
//         if (v1 > v2) std::swap(v1, v2);
//         for (auto& e : edges) {
//             if (e.v1 == v1 && e.v2 == v2) { e.tri2 = triIdx; return; }
//         }
//         edges.push_back({v1, v2, triIdx, -1});
//     };
//
//     for (size_t i = 0; i < triangles.size(); ++i) {
//         addEdge(triangles[i].vIdx[0], triangles[i].vIdx[1], i);
//         addEdge(triangles[i].vIdx[1], triangles[i].vIdx[2], i);
//         addEdge(triangles[i].vIdx[2], triangles[i].vIdx[0], i);
//     }
//
//     // Параметры камеры (как на скриншоте Blender)
//     float angleX = 0.15f, angleY = 0.0f;
//     Vec3 cameraPos = {0, -0.1f, -3.0f};
//     float scale = 380.0f;
//
//     std::vector<Vec3> transVertices(vertices.size());
//     std::vector<std::pair<int, int>> screenCoords(vertices.size());
//
//     // Трансформация вершин в пространство камеры
//     for (size_t i = 0; i < vertices.size(); ++i) {
//         Vec3 v = vertices[i];
//         float x1 = v.x * cosf(angleY) - v.z * sinf(angleY);
//         float z1 = v.x * sinf(angleY) + v.z * cosf(angleY);
//         float y2 = v.y * cosf(angleX) - z1 * sinf(angleX);
//         float z2 = v.y * sinf(angleX) + z1 * cosf(angleX);
//
//         transVertices[i] = {x1, y2, z2 + cameraPos.z};
//         screenCoords[i] = {
//             static_cast<int>(x1 * scale + WIDTH / 2),
//             static_cast<int>(y2 * scale + HEIGHT / 2)
//         };
//     }
//
//     // Предварительный расчет нормалей полигонов
//     for (auto& t : triangles) {
//         Vec3 e1 = transVertices[t.vIdx[1]] - transVertices[t.vIdx[0]];
//         Vec3 e2 = transVertices[t.vIdx[2]] - transVertices[t.vIdx[0]];
//         t.normal = e1.cross(e2).normalize();
//     }
//
//     std::vector<uint8_t> image(WIDTH * HEIGHT * 4, 255); // Белый холст
//     Vec3 viewDir = {0, 0, 1}; // Направление взгляда ортогонально плоскости экрана в нашей системе
//
//     // ФАЗА КЛАССИФИКАЦИИ И ГЕОМЕТРИЧЕСКОГО ОТСЕЧЕНИЯ
//     for (const auto& e : edges) {
//         if (e.tri1 == -1) continue;
//
//         bool isLineArtCandidate = false;
//         float dot1 = triangles[e.tri1].normal.dot(viewDir);
//         float dot2 = (e.tri2 != -1) ? triangles[e.tri2].normal.dot(viewDir) : -1.0f;
//
//         // ПРАВИЛО 1: Силуэт (граница видимого и невидимого полигона)
//         if ((dot1 >= 0.0f && dot2 < 0.0f) || (dot1 < 0.0f && dot2 >= 0.0f)) {
//             isLineArtCandidate = true;
//         }
//         // ПРАВИЛО 2: Залом / Складка (Crease) внутри видимой зоны
//         else if (e.tri2 != -1 && dot1 >= 0.0f && dot2 >= 0.0f) {
//             float angleBetween = triangles[e.tri1].normal.dot(triangles[e.tri2].normal);
//             if (angleBetween < 0.85f) { // Порог угла излома формы (~31 градус)
//                 isLineArtCandidate = true;
//             }
//         }
//
//         // Если ребро принципиально должно быть отрисовано, проверяем, не скрыто ли оно
//         if (isLineArtCandidate) {
//             Vec3 p1 = transVertices[e.v1];
//             Vec3 p2 = transVertices[e.v2];
//
//             // Берем 3 тестовые точки на ребре (начала, центр, конец) для точной проверки перекрытия
//             bool isVisible = true;
//             const int sampleCount = 3;
//
//             for (int s = 0; s < sampleCount; ++s) {
//                 float t_factor = (float)s / (sampleCount - 1);
//                 Vec3 samplePoint = p1 + (p2 - p1) * t_factor;
//
//                 // Стреляем лучом из точки ребра в сторону камеры (назад по оси Z)
//                 Vec3 rayOrigin = samplePoint;
//                 Vec3 rayDir = {0.0f, 0.0f, -1.0f};
//
//                 // Ищем, не пересекает ли этот луч какой-либо другой фронтальный треугольник
//                 for (const auto& t : triangles) {
//                     if (t.normal.dot(viewDir) < 0.0f) continue; // Игнорируем задние полигоны
//
//                     float intersectT;
//                     if (rayTriangleIntersect(rayOrigin, rayDir, transVertices[t.vIdx[0]], transVertices[t.vIdx[1]], transVertices[t.vIdx[2]], intersectT)) {
//                         // Если пересечение ближе, чем расстояние до плоскости камеры, точка скрыта
//                         if (intersectT > 0.001f) {
//                             isVisible = false;
//                             break;
//                         }
//                     }
//                 }
//                 if (!isVisible) break;
//             }
//
//             // Ребро прошло топологический отбор и тест на видимость — пускаем в печать!
//             if (isVisible) {
//                 int x0 = screenCoords[e.v1].first, y0 = screenCoords[e.v1].second;
//                 int x1 = screenCoords[e.v2].first, y1 = screenCoords[e.v2].second;
//                 drawLine(image, x0, y0, x1, y1, 4); // Жирный, уверенный штрих толщиной 4px
//             }
//         }
//     }
//
//     stbi_write_png("pure_vector_lineart.png", WIDTH, HEIGHT, 4, image.data(), WIDTH * 4);
//     std::cout << "Чистейший векторный Line Art сохранен в pure_vector_lineart.png!" << std::endl;
//
//     return 0;
// }







/*

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const int WIDTH = 1000;
const int HEIGHT = 1000;

struct Vec3 {
    float x, y, z;
    Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vec3 operator*(float f) const { return {x * f, y * f, z * f}; }
    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 cross(const Vec3& v) const { return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x}; }
    Vec3 normalize() const { float len = std::sqrt(x*x + y*y + z*z); return len > 0 ? Vec3{x/len, y/len, z/len} : Vec3{0,0,0}; }
};

struct Triangle {
    int vIdx[3];
    Vec3 faceNormal;
};

struct Edge {
    int v1, v2;
    int tri1 = -1;
    int tri2 = -1;
};

// Функция трассировки луча для проверки перекрытия (Occlusion)
bool rayTriangleIntersect(const Vec3& rayOrigin, const Vec3& rayDir, const Vec3& v0, const Vec3& v1, const Vec3& v2, float& t) {
    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;
    Vec3 pvec = rayDir.cross(edge2);
    float det = edge1.dot(pvec);
    if (std::abs(det) < 1e-5f) return false;
    float invDet = 1.0f / det;
    Vec3 tvec = rayOrigin - v0;
    float u = tvec.dot(pvec) * invDet;
    if (u < 0.0f || u > 1.0f) return false;
    Vec3 qvec = tvec.cross(edge1);
    float v = rayDir.dot(qvec) * invDet;
    if (v < 0.0f || u + v > 1.0f) return false;
    t = edge2.dot(qvec) * invDet;
    return t > 1e-4f;
}

// Рисование гладкой линии с Anti-Aliasing
void drawLineSmooth(std::vector<uint8_t>& image, int x0, int y0, int x1, int y1, int thickness) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        for (int ty = -thickness; ty <= thickness; ++ty) {
            for (int tx = -thickness; tx <= thickness; ++tx) {
                int nx = x0 + tx;
                int ny = y0 + ty;
                if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                    float dist = std::sqrt(tx * tx + ty * ty);
                    if (dist > thickness) continue;
                    int imgIdx = (ny * WIDTH + nx) * 4;
                    float alpha = 1.0f - (dist / thickness);
                    image[imgIdx + 0] = static_cast<uint8_t>(image[imgIdx + 0] * (1.0f - alpha));
                    image[imgIdx + 1] = static_cast<uint8_t>(image[imgIdx + 1] * (1.0f - alpha));
                    image[imgIdx + 2] = static_cast<uint8_t>(image[imgIdx + 2] * (1.0f - alpha));
                }
            }
        }
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx)  { err += dx; y0 += sy; }
    }
}

std::vector<Triangle> loadOBJ(const std::string& filename, std::vector<Vec3>& vertices) {
    std::vector<Triangle> triangles;
    std::ifstream file(filename);
    if (!file.is_open()) return triangles;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.push_back({x, y, z});
        } else if (type == "f") {
            std::string v1, v2, v3;
            ss >> v1 >> v2 >> v3;
            auto get_idx = [](const std::string& s) { return std::stoi(s.substr(0, s.find('/'))) - 1; };
            triangles.push_back({{get_idx(v1), get_idx(v2), get_idx(v3)}, {0,0,0}});
        }
    }
    return triangles;
}

int main() {
    std::vector<Vec3> vertices;
    std::vector<Triangle> triangles = loadOBJ("suzanne.obj", vertices);
    if (triangles.empty()) return 1;

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

    // Правильная ориентация камеры (корректируем переворот по Y/Z)
    float angleX = -0.2f; // Наклон вперед
    float angleY = 3.14159f; // Разворачиваем лицом к камере
    Vec3 cameraPos = {0, 0.2f, -3.0f};
    float scale = 400.0f;

    std::vector<Vec3> transVertices(vertices.size());
    std::vector<std::pair<int, int>> screenCoords(vertices.size());
    std::vector<Vec3> vertNormals(vertices.size(), Vec3{0, 0, 0});

    // Шаг 1: Трансформация геометрии
    for (size_t i = 0; i < vertices.size(); ++i) {
        Vec3 v = vertices[i];
        // В Blender Z - это вверх, исправляем матрицу поворота
        float x1 = v.x * cosf(angleY) - v.y * sinf(angleY);
        float y1 = v.x * sinf(angleY) + v.y * cosf(angleY);

        float y2 = y1 * cosf(angleX) - v.z * sinf(angleX);
        float z2 = y1 * sinf(angleX) + v.z * cosf(angleX);

        transVertices[i] = {x1, z2, -y2 + cameraPos.z};
        screenCoords[i] = {
            static_cast<int>(transVertices[i].x * scale + WIDTH / 2),
            static_cast<int>(-transVertices[i].y * scale + HEIGHT / 2) // Инвертируем Y для экрана
        };
    }

    // Шаг 2: Расчет нормалей граней и накопление СГЛАЖЕННЫХ нормалей вершин
    for (auto& t : triangles) {
        Vec3 e1 = transVertices[t.vIdx[1]] - transVertices[t.vIdx[0]];
        Vec3 e2 = transVertices[t.vIdx[2]] - transVertices[t.vIdx[0]];
        t.faceNormal = e1.cross(e2).normalize();

        vertNormals[t.vIdx[0]] = vertNormals[t.vIdx[0]] + t.faceNormal;
        vertNormals[t.vIdx[1]] = vertNormals[t.vIdx[1]] + t.faceNormal;
        vertNormals[t.vIdx[2]] = vertNormals[t.vIdx[2]] + t.faceNormal;
    }
    for (auto& n : vertNormals) n = n.normalize();

    std::vector<uint8_t> image(WIDTH * HEIGHT * 4, 255);
    Vec3 viewDir = {0, 0, 1};

    // Шаг 3: Фильтрация ребер по сглаженным нормалям вершин (как в Blender)
    for (const auto& e : edges) {
        if (e.tri1 == -1) continue;

        // Вместо жестких нормалей полигона берем среднюю нормаль вершин ребра
        Vec3 avgNormal = (vertNormals[e.v1] + vertNormals[e.v2]) * 0.5f;
        float dotView = avgNormal.normalize().dot(viewDir);

        // Строгий фильтр слабо наклоненных поверхностей:
        // Ребро признается силуэтным только в узком диапазоне перехода уклона к камере
        bool isSilhouette = (std::abs(dotView) < 0.25f);

        // Дополнительный тест: если это внутреннее ребро, отбрасываем плоские участки
        if (isSilhouette && e.tri2 != -1) {
            float edgeAngle = triangles[e.tri1].faceNormal.dot(triangles[e.tri2].faceNormal);
            if (edgeAngle > 0.92f) { // Отбрасываем все "слабо наклоненные" и плоские стыки
                isSilhouette = false;
            }
        }

        if (isSilhouette) {
            Vec3 p1 = transVertices[e.v1];
            Vec3 p2 = transVertices[e.v2];
            bool isVisible = true;
            const int sampleCount = 3;

            for (int s = 0; s < sampleCount; ++s) {
                float t_factor = (float)s / (sampleCount - 1);
                Vec3 samplePoint = p1 + (p2 - p1) * t_factor;
                Vec3 rayOrigin = samplePoint;
                Vec3 rayDir = {0.0f, 0.0f, -1.0f};

                for (const auto& t : triangles) {
                    if (t.faceNormal.dot(viewDir) < 0.0f) continue;
                    float intersectT;
                    if (rayTriangleIntersect(rayOrigin, rayDir, transVertices[t.vIdx[0]], transVertices[t.vIdx[1]], transVertices[t.vIdx[2]], intersectT)) {
                        if (intersectT > 0.01f) {
                            isVisible = false;
                            break;
                        }
                    }
                }
                if (!isVisible) break;
            }

            if (isVisible) {
                int x0 = screenCoords[e.v1].first, y0 = screenCoords[e.v1].second;
                int x1 = screenCoords[e.v2].first, y1 = screenCoords[e.v2].second;
                drawLineSmooth(image, x0, y0, x1, y1, 3); // Сочный, чистый штрих
            }
        }
    }

    stbi_write_png("clean_blender_lineart.png", WIDTH, HEIGHT, 4, image.data(), WIDTH * 4);
    std::cout << "Готово! Результат в clean_blender_lineart.png" << std::endl;
    return 0;
}*/



















#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const int WIDTH = 1000;
const int HEIGHT = 1000;

struct Vec3 {
    float x, y, z;
    Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vec3 operator*(float f) const { return {x * f, y * f, z * f}; }
    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 cross(const Vec3& v) const { return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x}; }
    Vec3 normalize() const { float len = std::sqrt(x*x + y*y + z*z); return len > 0 ? Vec3{x/len, y/len, z/len} : Vec3{0,0,0}; }
};

struct Triangle {
    int vIdx[3];
    Vec3 faceNormal;
    bool isFrontFacing;
};

struct Edge {
    int v1, v2;
    int tri1 = -1;
    int tri2 = -1;
};

struct ViewAngle {
    std::string name;
    float rotX;
    float rotY;
};

void drawLineSmooth(std::vector<uint8_t>& image, int x0, int y0, int x1, int y1, int thickness) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        for (int ty = -thickness; ty <= thickness; ++ty) {
            for (int tx = -thickness; tx <= thickness; ++tx) {
                int nx = x0 + tx;
                int ny = y0 + ty;
                if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                    float dist = std::sqrt(tx * tx + ty * ty);
                    if (dist > thickness) continue;
                    int imgIdx = (ny * WIDTH + nx) * 4;
                    float alpha = 1.0f - (dist / thickness);

                    image[imgIdx + 0] = static_cast<uint8_t>(image[imgIdx + 0] * (1.0f - alpha));
                    image[imgIdx + 1] = static_cast<uint8_t>(image[imgIdx + 1] * (1.0f - alpha));
                    image[imgIdx + 2] = static_cast<uint8_t>(image[imgIdx + 2] * (1.0f - alpha));
                }
            }
        }
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx)  { err += dx; y0 += sy; }
    }
}

// Улучшенный загрузчик с автоматической триангуляцией полигонов (Quads -> Triangles)
std::vector<Triangle> loadOBJ(const std::string& filename, std::vector<Vec3>& vertices) {
    std::vector<Triangle> triangles;
    std::ifstream file(filename);
    if (!file.is_open()) return triangles;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;
        if (type == "v") {
            float x, y, z;
            ss >> x >> y >> z;
            vertices.push_back({x, y, z});
        } else if (type == "f") {
            std::vector<std::string> tokens;
            std::string token;
            while (ss >> token) {
                tokens.push_back(token);
            }
            auto get_idx = [](const std::string& s) { return std::stoi(s.substr(0, s.find('/'))) - 1; };

            if (tokens.size() >= 3) {
                int idx0 = get_idx(tokens[0]);
                int idx1 = get_idx(tokens[1]);
                int idx2 = get_idx(tokens[2]);
                triangles.push_back({{idx0, idx1, idx2}, {0,0,0}, false});

                // Если это Quad (четырехугольник), добавляем второй треугольник
                if (tokens.size() == 4) {
                    int idx3 = get_idx(tokens[3]);
                    triangles.push_back({{idx0, idx2, idx3}, {0,0,0}, false});
                }
            }
        }
    }
    return triangles;
}

int main() {
    std::vector<Vec3> baseVertices;
    std::vector<Triangle> triangles = loadOBJ("suzanne.obj", baseVertices);
    if (triangles.empty()) {
        std::cerr << "Положите файл suzanne.obj рядом с бинарником!" << std::endl;
        return 1;
    }

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

    std::vector<ViewAngle> views = {
        {"1_front",   0.0f,  0.0f},
        {"2_back",    0.0f,  3.14159f},
        {"3_left",    0.0f,  4.71239f},
        {"4_right",   0.0f,  1.5708f},
        {"5_top",     1.5708f, 0.0f},
        {"6_bottom", -1.5708f, 0.0f}
    };

    float scale = 400.0f;

    for (const auto& view : views) {
        std::vector<Vec3> transVertices(baseVertices.size());
        std::vector<std::pair<int, int>> screenCoords(baseVertices.size());

        for (size_t i = 0; i < baseVertices.size(); ++i) {
            Vec3 v = baseVertices[i];
            float x1 = v.x * cosf(view.rotY) + v.z * sinf(view.rotY);
            float z1 = -v.x * sinf(view.rotY) + v.z * cosf(view.rotY);
            float y2 = v.y * cosf(view.rotX) - z1 * sinf(view.rotX);
            float z2 = v.y * sinf(view.rotX) + z1 * cosf(view.rotX);

            transVertices[i] = {x1, y2, z2};
            screenCoords[i] = {
                static_cast<int>(x1 * scale + WIDTH / 2),
                static_cast<int>(-y2 * scale + HEIGHT / 2)
            };
        }

        for (auto& t : triangles) {
            Vec3 e1 = transVertices[t.vIdx[1]] - transVertices[t.vIdx[0]];
            Vec3 e2 = transVertices[t.vIdx[2]] - transVertices[t.vIdx[0]];
            t.faceNormal = e1.cross(e2).normalize();
            t.isFrontFacing = (t.faceNormal.z > 0.0f);
        }

        // Рендеринг Z-буфера со всеми полигонами
        std::vector<float> zBuffer(WIDTH * HEIGHT, -1e9f);
        for (const auto& t : triangles) {
            if (!t.isFrontFacing) continue;

            int x0 = screenCoords[t.vIdx[0]].first, y0 = screenCoords[t.vIdx[0]].second;
            int x1 = screenCoords[t.vIdx[1]].first, y1 = screenCoords[t.vIdx[1]].second;
            int x2 = screenCoords[t.vIdx[2]].first, y2 = screenCoords[t.vIdx[2]].second;

            int minX = std::max(0, std::min({x0, x1, x2}));
            int maxX = std::min(WIDTH - 1, std::max({x0, x1, x2}));
            int minY = std::max(0, std::min({y0, y1, y2}));
            int maxY = std::min(HEIGHT - 1, std::max({y0, y1, y2}));

            float d = static_cast<float>((y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2));
            if (std::abs(d) < 1e-5f) continue;

            for (int py = minY; py <= maxY; ++py) {
                for (int px = minX; px <= maxX; ++px) {
                    float w0 = ((y1 - y2) * (px - x2) + (x2 - x1) * (py - y2)) / d;
                    float w1 = ((y2 - y0) * (px - x2) + (x0 - x2) * (py - y2)) / d;
                    float w2 = 1.0f - w0 - w1;

                    if (w0 >= -1e-3f && w1 >= -1e-3f && w2 >= -1e-3f) {
                        float z = w0 * transVertices[t.vIdx[0]].z +
                        w1 * transVertices[t.vIdx[1]].z +
                        w2 * transVertices[t.vIdx[2]].z;
                        int idx = py * WIDTH + px;
                        if (z > zBuffer[idx]) {
                            zBuffer[idx] = z;
                        }
                    }
                }
            }
        }

        std::vector<uint8_t> image(WIDTH * HEIGHT * 4, 255);

        for (const auto& e : edges) {
            bool isSilhouette = false;
            bool isCrease = false;

            bool f1 = (e.tri1 != -1) ? triangles[e.tri1].isFrontFacing : false;
            bool f2 = (e.tri2 != -1) ? triangles[e.tri2].isFrontFacing : false;

            if (e.tri2 == -1) {
                isSilhouette = f1;
            } else {
                if (f1 != f2) isSilhouette = true;
            }

            // Настройка строгости отсечения (угол складки повышен до ~80 градусов)
            if (!isSilhouette && e.tri1 != -1 && e.tri2 != -1 && f1 && f2) {
                float dotNormal = triangles[e.tri1].faceNormal.dot(triangles[e.tri2].faceNormal);
                if (dotNormal < 0.17f) { // Оставляем только радикально острые углы
                    isCrease = true;
                }
            }

            if (isSilhouette || isCrease) {
                int x0 = screenCoords[e.v1].first, y0 = screenCoords[e.v1].second;
                int x1 = screenCoords[e.v2].first, y1 = screenCoords[e.v2].second;
                float z0 = transVertices[e.v1].z;
                float z1 = transVertices[e.v2].z;

                bool isVisible = true;
                const int steps = 12; // Повысили точность шага для Z-теста тонких линий
                for (int s = 0; s <= steps; ++s) {
                    float t = (float)s / steps;
                    int currX = static_cast<int>(x0 + (x1 - x0) * t);
                    int currY = static_cast<int>(y0 + (y1 - y0) * t);
                    float currZ = z0 + (z1 - z0) * t;

                    if (currX >= 0 && currX < WIDTH && currY >= 0 && currY < HEIGHT) {
                        int bufferIdx = currY * WIDTH + currX;
                        // Уменьшенный bias (0.005f), чтобы ребра не «вгрызались» в собственные полигоны
                        if (zBuffer[bufferIdx] > currZ + 0.005f) {
                            isVisible = false;
                            break;
                        }
                    }
                }

                if (isVisible) {
                    int thickness = isSilhouette ? 2 : 1;
                    drawLineSmooth(image, x0, y0, x1, y1, thickness);
                }
            }
        }

        std::string filename = "lineart_" + view.name + ".png";
        stbi_write_png(filename.c_str(), WIDTH, HEIGHT, 4, image.data(), WIDTH * 4);
        std::cout << "[Успех] Сохранен чистый ракурс: " << filename << std::endl;
    }

    return 0;
}
