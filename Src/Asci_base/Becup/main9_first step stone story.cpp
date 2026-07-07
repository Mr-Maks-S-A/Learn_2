// #include <iostream>
// #include <vector>
// #include <cmath>
// #include <algorithm>
// #include <cstdint>
//
// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include "stb_image_write.h"
//
// const int WIDTH = 1000;
// const int HEIGHT = 1000;
// const float M_PI_F = 3.14159265358979323846f;
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
// // Генератор тора (бублика)
// // R - большой радиус, r - малый радиус трубы
// // radialSegments и tubularSegments - плотность сетки (как в Blender)
// std::vector<Triangle> createTorus(float R, float r, int radialSegments, int tubularSegments) {
//     std::vector<Triangle> mesh;
//     std::vector<std::vector<Vec3>> vertices(radialSegments, std::vector<Vec3>(tubularSegments));
//
//     // 1. Генерируем вершины тора
//     for (int i = 0; i < radialSegments; ++i) {
//         float u = (float)i / radialSegments * 2.0f * M_PI_F;
//         for (int j = 0; j < tubularSegments; ++j) {
//             float v = (float)j / tubularSegments * 2.0f * M_PI_F;
//
//             float x = (R + r * cos(v)) * cos(u);
//             float y = (R + r * cos(v)) * sin(u);
//             float z = r * sin(v);
//
//             vertices[i][j] = {x, y, z};
//         }
//     }
//
//     // 2. Связываем вершины в треугольники (каждый шаг сетки — это квад из двух треугольников)
//     for (int i = 0; i < radialSegments; ++i) {
//         int next_i = (i + 1) % radialSegments;
//         for (int j = 0; j < tubularSegments; ++j) {
//             int next_j = (j + 1) % tubularSegments;
//
//             Vec3 v0 = vertices[i][j];
//             Vec3 v1 = vertices[next_i][j];
//             Vec3 v2 = vertices[i][next_j];
//             Vec3 v3 = vertices[next_i][next_j];
//
//             // Первый треугольник квада
//             Triangle t1; t1.v[0] = v0; t1.v[1] = v1; t1.v[2] = v2;
//             t1.normal = (t1.v[1] - t1.v[0]).cross(t1.v[2] - t1.v[0]).normalize();
//             mesh.push_back(t1);
//
//             // Второй треугольник квада
//             Triangle t2; t2.v[0] = v1; t2.v[1] = v3; t2.v[2] = v2;
//             t2.normal = (t2.v[1] - t2.v[0]).cross(t2.v[2] - t2.v[0]).normalize();
//             mesh.push_back(t2);
//         }
//     }
//     return mesh;
// }
//
// // Генератор одного плоского треугольника
// std::vector<Triangle> createSingleTriangle() {
//     std::vector<Triangle> mesh;
//     Triangle t;
//     // Координаты вершин треугольника (немного сместим его в сторону)
//     t.v[0] = { 1.5f, -1.0f, 0.0f };
//     t.v[1] = { 3.0f,  1.0f, 0.5f };
//     t.v[2] = { 2.0f, -1.5f, -0.5f };
//     t.normal = (t.v[1] - t.v[0]).cross(t.v[2] - t.v[0]).normalize();
//     mesh.push_back(t);
//     return mesh;
// }
//
// int main() {
//     std::vector<Vec3> normalBuffer(WIDTH * HEIGHT, Vec3{0, 0, 0});
//     std::vector<float> zBuffer(WIDTH * HEIGHT, -std::numeric_limits<float>::max());
//     std::vector<uint8_t> image(WIDTH * HEIGHT * 4, 255); // Белый фон
//
//     // Объединяем всю геометрию в одну сцену
//     std::vector<Triangle> sceneMesh;
//
//     // Создаем тор (R=1.3, r=0.5, 40 на 20 сегментов для плавности)
//     auto torus = createTorus(1.3f, 0.5f, 40, 20);
//     sceneMesh.insert(sceneMesh.end(), torus.begin(), torus.end());
//
//     // Создаем отдельный треугольник
//     auto triangle = createSingleTriangle();
//     sceneMesh.insert(sceneMesh.end(), triangle.begin(), triangle.end());
//
//     // Углы поворота сцены для красивого ракурса
//     float angleX = 0.7f;
//     float angleY = 0.5f;
//     Vec3 cameraPos = {0, 0, -6}; // Камера чуть дальше, так как тор крупнее куба
//
//     // 1. РЕНДЕРИНГ ГЕОМЕТРИИ В БУФЕРЫ
//     for (const auto& t : sceneMesh) {
//         Triangle transformed = t;
//
//         for (int i = 0; i < 3; ++i) {
//             // Вращение вокруг оси Y
//             float x1 = transformed.v[i].x * cos(angleY) - transformed.v[i].z * sin(angleY);
//             float z1 = transformed.v[i].x * sin(angleY) + transformed.v[i].z * cos(angleY);
//
//             // Вращение вокруг оси X
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
//         // Проекция на экран
//         int x[3], y[3];
//         float scale = 250.0f; // Масштаб отображения
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
//                 if (std::abs(d) < 0.00001f) continue; // Защита от вырожденных треугольников
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
//     // 2. ПОСТ-ОБРАБОТКА (LINE ART ФИЛЬТР)
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
//             // Если перепад нормалей значительный — рисуем линию контура
//             // Порог 0.15f делает линии аккуратными на гладком торе
//             if (edge > 0.15f) {
//                 image[imgIdx + 0] = 0;
//                 image[imgIdx + 1] = 0;
//                 image[imgIdx + 2] = 0;
//             } else {
//                 Vec3 currentNormal = normalBuffer[y * WIDTH + x];
//                 if (currentNormal.dot(currentNormal) > 0.1f) {
//                     // Заливка полигонов легким серым цветом, как в Blender Solid View
//                     image[imgIdx + 0] = 235;
//                     image[imgIdx + 1] = 235;
//                     image[imgIdx + 2] = 235;
//                 }
//             }
//         }
//     }
//
//     stbi_write_png("torus_triangle_output.png", WIDTH, HEIGHT, 4, image.data(), WIDTH * 4);
//     std::cout << "Рендер тора и треугольника успешно сохранен в torus_triangle_output.png!" << std::endl;
//
//     return 0;
// }












#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

const int WIDTH = 1000;
const int HEIGHT = 1000;
const float M_PI_F = 3.14159265358979323846f;

struct Vec3 {
    float x, y, z;
    Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vec3 operator*(float f) const { return {x * f, y * f, z * f}; }
    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 cross(const Vec3& v) const {
        return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
    }
    Vec3 normalize() const {
        float len = std::sqrt(x*x + y*y + z*z);
        return len > 0 ? Vec3{x/len, y/len, z/len} : Vec3{0,0,0};
    }
};

struct Triangle {
    Vec3 v[3];
    Vec3 normal;
};

// Генератор тора (бублика)
std::vector<Triangle> createTorus(float R, float r, int radialSegments, int tubularSegments) {
    std::vector<Triangle> mesh;
    std::vector<std::vector<Vec3>> vertices(radialSegments, std::vector<Vec3>(tubularSegments));

    for (int i = 0; i < radialSegments; ++i) {
        float u = (float)i / radialSegments * 2.0f * M_PI_F;
        for (int j = 0; j < tubularSegments; ++j) {
            float v = (float)j / tubularSegments * 2.0f * M_PI_F;

            float x = (R + r * cosf(v)) * cosf(u);
            float y = (R + r * cosf(v)) * sinf(u);
            float z = r * sinf(v);

            vertices[i][j] = {x, y, z};
        }
    }

    for (int i = 0; i < radialSegments; ++i) {
        int next_i = (i + 1) % radialSegments;
        for (int j = 0; j < tubularSegments; ++j) {
            int next_j = (j + 1) % tubularSegments;

            Vec3 v0 = vertices[i][j];
            Vec3 v1 = vertices[next_i][j];
            Vec3 v2 = vertices[i][next_j];
            Vec3 v3 = vertices[next_i][next_j];

            Triangle t1; t1.v[0] = v0; t1.v[1] = v1; t1.v[2] = v2;
            t1.normal = (t1.v[1] - t1.v[0]).cross(t1.v[2] - t1.v[0]).normalize();
            mesh.push_back(t1);

            Triangle t2; t2.v[0] = v1; t2.v[1] = v3; t2.v[2] = v2;
            t2.normal = (t2.v[1] - t2.v[0]).cross(t2.v[2] - t2.v[0]).normalize();
            mesh.push_back(t2);
        }
    }
    return mesh;
}

// Генератор одного плоского треугольника
std::vector<Triangle> createSingleTriangle() {
    std::vector<Triangle> mesh;
    Triangle t;
    t.v[0] = { 1.8f, -1.2f, 0.0f };
    t.v[1] = { 3.2f,  0.8f, 0.5f };
    t.v[2] = { 2.2f, -1.7f, -0.5f };
    t.normal = (t.v[1] - t.v[0]).cross(t.v[2] - t.v[0]).normalize();
    mesh.push_back(t);
    return mesh;
}

// Генератор обезьянки Сюзанны (Low-Poly)
std::vector<Triangle> createSuzanne() {
    std::vector<Vec3> vertices = {
        {0.0f, 0.9f, 0.2f},    {-0.5f, 0.7f, 0.4f},   {0.5f, 0.7f, 0.4f},
        {-0.6f, 0.3f, 0.7f},   {-0.2f, 0.3f, 0.8f},   {0.2f, 0.3f, 0.8f},
        {0.6f, 0.3f, 0.7f},    {-0.4f, 0.0f, 0.7f},   {0.4f, 0.0f, 0.7f},
        {0.0f, -0.1f, 0.9f},   {-0.3f, -0.4f, 0.8f},  {0.3f, -0.4f, 0.8f},
        {0.0f, -0.6f, 0.6f},   {-0.9f, 0.4f, -0.1f},  {-1.2f, 0.1f, -0.2f},
        {-0.9f, -0.2f, -0.1f}, {0.9f, 0.4f, -0.1f},   {1.2f, 0.1f, -0.2f},
        {0.9f, -0.2f, -0.1f},  {0.0f, 0.5f, -0.8f},   {0.0f, -0.4f, -0.7f}
    };

    std::vector<std::vector<int>> indices = {
        {0, 1, 4}, {0, 4, 5}, {0, 5, 2}, {1, 3, 4}, {2, 5, 6},
        {4, 3, 7}, {4, 7, 9}, {5, 9, 8}, {5, 8, 6}, {7, 10, 9},
        {8, 9, 11}, {10, 12, 9}, {11, 9, 12}, {1, 13, 3}, {13, 14, 3},
        {3, 14, 15}, {3, 15, 7}, {7, 15, 10}, {2, 6, 16}, {16, 6, 17},
        {6, 17, 18}, {6, 18, 8}, {8, 18, 11}, {0, 19, 1}, {0, 2, 19},
        {13, 19, 14}, {16, 17, 19}, {14, 20, 15}, {17, 18, 20}, {19, 20, 14},
        {19, 17, 20}, {15, 20, 12}, {18, 12, 20}, {10, 15, 12}, {11, 12, 18}
    };

    std::vector<Triangle> mesh;
    for (const auto& tri : indices) {
        Triangle t;
        // Немного сместим Сюзанну влево (-1.5f), чтобы она не перекрывала тор в центре
        t.v[0] = vertices[tri[0]] + Vec3{-1.5f, 0.0f, 0.0f};
        t.v[1] = vertices[tri[1]] + Vec3{-1.5f, 0.0f, 0.0f};
        t.v[2] = vertices[tri[2]] + Vec3{-1.5f, 0.0f, 0.0f};
        t.normal = (t.v[1] - t.v[0]).cross(t.v[2] - t.v[0]).normalize();
        mesh.push_back(t);
    }
    return mesh;
}

int main() {
    std::vector<Vec3> normalBuffer(WIDTH * HEIGHT, Vec3{0, 0, 0});
    std::vector<float> zBuffer(WIDTH * HEIGHT, -std::numeric_limits<float>::max());
    std::vector<uint8_t> image(WIDTH * HEIGHT * 4, 255);

    std::vector<Triangle> sceneMesh;

    // 1. Добавляем тор (сдвинем его чуть вправо для композиции)
    auto torus = createTorus(1.0f, 0.4f, 40, 20);
    for (auto& t : torus) {
        t.v[0] = t.v[0] + Vec3{0.7f, 0.0f, 0.0f};
        t.v[1] = t.v[1] + Vec3{0.7f, 0.0f, 0.0f};
        t.v[2] = t.v[2] + Vec3{0.7f, 0.0f, 0.0f};
    }
    sceneMesh.insert(sceneMesh.end(), torus.begin(), torus.end());

    // 2. Добавляем отдельный треугольник
    auto triangle = createSingleTriangle();
    sceneMesh.insert(sceneMesh.end(), triangle.begin(), triangle.end());

    // 3. Добавляем Сюзанну
    auto suzanne = createSuzanne();
    sceneMesh.insert(sceneMesh.end(), suzanne.begin(), suzanne.end());

    // Общая трансформация камеры
    float angleX = 0.4f;
    float angleY = 0.5f;
    Vec3 cameraPos = {0, 0, -5.5f};

    // 1. РЕНДЕРИНГ ГЕОМЕТРИИ В БУФЕРЫ
    for (const auto& t : sceneMesh) {
        Triangle transformed = t;

        for (int i = 0; i < 3; ++i) {
            float x1 = transformed.v[i].x * cosf(angleY) - transformed.v[i].z * sinf(angleY);
            float z1 = transformed.v[i].x * sinf(angleY) + transformed.v[i].z * cosf(angleY);

            float y2 = transformed.v[i].y * cosf(angleX) - z1 * sinf(angleX);
            float z2 = transformed.v[i].y * sinf(angleX) + z1 * cosf(angleX);

            transformed.v[i] = {x1, y2, z2 + cameraPos.z};
        }

        Vec3 e1 = transformed.v[1] - transformed.v[0];
        Vec3 e2 = transformed.v[2] - transformed.v[0];
        Vec3 norm = e1.cross(e2).normalize();

        int x[3], y[3];
        float scale = 230.0f;
        for (int i = 0; i < 3; ++i) {
            x[i] = static_cast<int>(transformed.v[i].x * scale + WIDTH / 2);
            y[i] = static_cast<int>(transformed.v[i].y * scale + HEIGHT / 2);
        }

        int minX = std::max(0, std::min({x[0], x[1], x[2]}));
        int maxX = std::min(WIDTH - 1, std::max({x[0], x[1], x[2]}));
        int minY = std::max(0, std::min({y[0], y[1], y[2]}));
        int maxY = std::min(HEIGHT - 1, std::max({y[0], y[1], y[2]}));

        for (int py = minY; py <= maxY; ++py) {
            for (int px = minX; px <= maxX; ++px) {
                float d = (y[1] - y[2]) * (x[0] - x[2]) + (x[2] - x[1]) * (y[0] - y[2]);
                if (std::abs(d) < 0.00001f) continue;

                float w0 = ((y[1] - y[2]) * (px - x[2]) + (x[2] - x[1]) * (py - y[2])) / d;
                float w1 = ((y[2] - y[0]) * (px - x[2]) + (x[0] - x[2]) * (py - y[2])) / d;
                float w2 = 1.0f - w0 - w1;

                if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                    float z = w0 * transformed.v[0].z + w1 * transformed.v[1].z + w2 * transformed.v[2].z;
                    int idx = py * WIDTH + px;
                    if (z > zBuffer[idx]) {
                        zBuffer[idx] = z;
                        normalBuffer[idx] = norm;
                    }
                }
            }
        }
    }

    // 2. ПОСТ-ОБРАБОТКА (LINE ART ФИЛЬТР СОБЕЛЯ)
    for (int y = 1; y < HEIGHT - 1; ++y) {
        for (int x = 1; x < WIDTH - 1; ++x) {
            Vec3 n_up    = normalBuffer[(y - 1) * WIDTH + x];
            Vec3 n_down  = normalBuffer[(y + 1) * WIDTH + x];
            Vec3 n_left  = normalBuffer[y * WIDTH + (x - 1)];
            Vec3 n_right = normalBuffer[y * WIDTH + (x + 1)];

            float edgeX = (n_right - n_left).dot(n_right - n_left);
            float edgeY = (n_up - n_down).dot(n_up - n_down);
            float edge = edgeX + edgeY;

            int imgIdx = (y * WIDTH + x) * 4;

            // Для низкополигональной Сюзанны порог излома 0.18f отлично выделит ребра лоуполи-сетки
            if (edge > 0.18f) {
                image[imgIdx + 0] = 0;
                image[imgIdx + 1] = 0;
                image[imgIdx + 2] = 0;
            } else {
                Vec3 currentNormal = normalBuffer[y * WIDTH + x];
                if (currentNormal.dot(currentNormal) > 0.1f) {
                    image[imgIdx + 0] = 235;
                    image[imgIdx + 1] = 235;
                    image[imgIdx + 2] = 235;
                }
            }
        }
    }

    stbi_write_png("scene_with_suzanne_output.png", WIDTH, HEIGHT, 4, image.data(), WIDTH * 4);
    std::cout << "Рендер сцены с Сюзанной успешно сохранен в scene_with_suzanne_output.png!" << std::endl;

    return 0;
}
