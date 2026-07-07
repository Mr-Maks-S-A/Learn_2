#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>
#include <algorithm>
#include <string>
#include <cfloat>

const int WIDTH = 200;
const int HEIGHT = 50;
const float ASPECT_RATIO = 3.15f;

struct Vec3 {
    float x, y, z;
    Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 cross(const Vec3& v) const {
        return { y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x };
    }
    void normalize() {
        float len = std::sqrt(x*x + y*y + z*z);
        if (len > 0) { x /= len; y /= len; z /= len; }
    }
};

struct Triangle {
    Vec3 p[3];
};

Vec3 rotateX(Vec3 p, float angle) {
    float s = std::sin(angle), c = std::cos(angle);
    return { p.x, p.y * c - p.z * s, p.y * s + p.z * c };
}

Vec3 rotateY(Vec3 p, float angle) {
    float s = std::sin(angle), c = std::cos(angle);
    return { p.x * c + p.z * s, p.y, -p.x * s + p.z * c };
}

Vec3 project(Vec3 p, float zoom) {
    // zoom теперь регулирует масштаб проекции
    float x = WIDTH / 2.0f + (p.x * zoom) / p.z * ASPECT_RATIO;
    float y = HEIGHT / 2.0f + (p.y * zoom) / p.z;
    return { x, y, p.z };
}

// Улучшенная проверка: чуть мягче границы для устранения дыр на стыках
bool barycentric(Vec3 A, Vec3 B, Vec3 C, Vec3 P, float& u, float& v, float& w) {
    Vec3 v0 = B - A, v1 = C - A, v2 = P - A;
    float d00 = v0.dot(v0);
    float d01 = v0.dot(v1);
    float d11 = v1.dot(v1);
    float d20 = v2.dot(v0);
    float d21 = v2.dot(v1);
    float denom = d00 * d11 - d01 * d01;
    if (std::abs(denom) < 1e-5) return false;
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1.0f - v - w;
    // Разрешаем крошечную погрешность
    return (u > -0.0001f && v > -0.0001f && w > -0.0001f);
}

// void rasterizeTriangle(Triangle t, char symbol, std::vector<char>& screen, std::vector<float>& zBuffer) {
//     int minX = std::max(0, (int)std::floor(std::min({t.p[0].x, t.p[1].x, t.p[2].x})));
//     int maxX = std::min(WIDTH - 1, (int)std::ceil(std::max({t.p[0].x, t.p[1].x, t.p[2].x})));
//     int minY = std::max(0, (int)std::floor(std::min({t.p[0].y, t.p[1].y, t.p[2].y})));
//     int maxY = std::min(HEIGHT - 1, (int)std::ceil(std::max({t.p[0].y, t.p[1].y, t.p[2].y})));

//     for (int y = minY; y <= maxY; ++y) {
//         for (int x = minX; x <= maxX; ++x) {
//             float u, v, w;
//             Vec3 P = {(float)x, (float)y, 0};
//             if (barycentric(t.p[0], t.p[1], t.p[2], P, u, v, w)) {
//                 float interpolatedZ = u * t.p[0].z + v * t.p[1].z + w * t.p[2].z;
//                 int idx = y * WIDTH + x;
//                 if (interpolatedZ < zBuffer[idx]) {
//                     zBuffer[idx] = interpolatedZ;
//                     screen[idx] = symbol;
//                 }
//             }
//         }
//     }
// }

void rasterizeTriangle(Triangle t, char symbol, std::vector<char>& screen, std::vector<float>& zBuffer) {
    int minX = std::max(0, (int)std::floor(std::min({t.p[0].x, t.p[1].x, t.p[2].x})));
    int maxX = std::min(WIDTH - 1, (int)std::ceil(std::max({t.p[0].x, t.p[1].x, t.p[2].x})));
    int minY = std::max(0, (int)std::floor(std::min({t.p[0].y, t.p[1].y, t.p[2].y})));
    int maxY = std::min(HEIGHT - 1, (int)std::ceil(std::max({t.p[0].y, t.p[1].y, t.p[2].y})));

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            float u, v, w;
            Vec3 P = {(float)x, (float)y, 0};
            if (barycentric(t.p[0], t.p[1], t.p[2], P, u, v, w)) {
                float interpolatedZ = u * t.p[0].z + v * t.p[1].z + w * t.p[2].z;
                int idx = y * WIDTH + x;
                
                if (interpolatedZ < zBuffer[idx]) {
                    zBuffer[idx] = interpolatedZ;
                    
                    // ЭФФЕКТ КОНТУРА (Wireframe):
                    // Если пиксель лежит на самом краю треугольника — рисуем яркую границу
                    float edgeThreshold = 0.05f; // Чем меньше число, тем тоньше линии
                    if (u < edgeThreshold || v < edgeThreshold || w < edgeThreshold) {
                        screen[idx] = '#'; // Символ для каркаса (сделай его контрастным)
                    } else {
                        screen[idx] = symbol; // Иначе — обычная заливка светом
                        // Или напиши screen[idx] = ' '; если хочешь ТОЛЬКО прозрачный каркас!
                    }
                }
            }
        }
    }
}



// char getAsciiChar(float brightness) {
//     std::string chars = " .:-=+*#%@";
//     int index = static_cast<int>(brightness * (chars.size() - 1));
//     return chars[std::max(0, std::min(index, (int)chars.size() - 1))];
// }

char getAsciiChar(float brightness) {
    // Более плавная и предсказуемая шкала плотности
    std::string chars = " .:-=+*I%%@#"; 
    int index = static_cast<int>(brightness * (chars.size() - 1));
    return chars[std::max(0, std::min(index, (int)chars.size() - 1))];
}

// Генерация Торуса (бублика)
std::vector<Triangle> generateTorus(float R, float r, int plots, int rings) {
    std::vector<Triangle> mesh;
    for (int i = 0; i < rings; i++) {
        for (int j = 0; j < plots; j++) {
            auto getPos = [&](int thetaIdx, int phiIdx) -> Vec3 {
                float theta = (float)thetaIdx * 2.0f * 3.14159f / rings;
                float phi = (float)phiIdx * 2.0f * 3.14159f / plots;
                return {
                    (R + r * std::cos(phi)) * std::cos(theta),
                    (R + r * std::cos(phi)) * std::sin(theta),
                    r * std::sin(phi)
                };
            };

            Vec3 p1 = getPos(i, j);
            Vec3 p2 = getPos(i + 1, j);
            Vec3 p3 = getPos(i + 1, j + 1);
            Vec3 p4 = getPos(i, j + 1);
            mesh.push_back({p1, p2, p3});
            mesh.push_back({p1, p3, p4});
        }
    }
    return mesh;
}

// Генерация Куба
std::vector<Triangle> generateCube(float size) {
    float h = size / 2.0f;
    Vec3 v[8] = {
        {-h, -h, -h}, { h, -h, -h}, { h,  h, -h}, {-h,  h, -h},
        {-h, -h,  h}, { h, -h,  h}, { h,  h,  h}, {-h,  h,  h}
    };
    std::vector<Triangle> mesh;
    mesh.push_back({v[0], v[2], v[1]}); mesh.push_back({v[0], v[3], v[2]});
    mesh.push_back({v[4], v[5], v[6]}); mesh.push_back({v[4], v[6], v[7]});
    mesh.push_back({v[0], v[4], v[7]}); mesh.push_back({v[0], v[7], v[3]});
    mesh.push_back({v[1], v[2], v[6]}); mesh.push_back({v[1], v[6], v[5]});
    mesh.push_back({v[3], v[7], v[6]}); mesh.push_back({v[3], v[6], v[2]});
    mesh.push_back({v[0], v[1], v[5]}); mesh.push_back({v[0], v[5], v[4]});
    return mesh;
}

// Генерация Сферы
std::vector<Triangle> generateSphere(float radius, int lats, int longs) {
    std::vector<Triangle> mesh;
    for (int i = 0; i < lats; i++) {
        float lat0 = 3.14159f * (-0.5f + (float)(i) / lats);
        float z0  = std::sin(lat0);
        float zr0 = std::cos(lat0);

        float lat1 = 3.14159f * (-0.5f + (float)(i + 1) / lats);
        float z1  = std::sin(lat1);
        float zr1 = std::cos(lat1);

        for (int j = 0; j < longs; j++) {
            float lng0 = 2.0f * 3.14159f * (float)(j) / longs;
            float x0 = std::cos(lng0);
            float y0 = std::sin(lng0);

            float lng1 = 2.0f * 3.14159f * (float)(j + 1) / longs;
            float x1 = std::cos(lng1);
            float y1 = std::sin(lng1);

            Vec3 p1 = {radius * x0 * zr0, radius * y0 * zr0, radius * z0};
            Vec3 p2 = {radius * x1 * zr0, radius * y1 * zr0, radius * z0};
            Vec3 p3 = {radius * x1 * zr1, radius * y1 * zr1, radius * z1};
            Vec3 p4 = {radius * x0 * zr1, radius * y0 * zr1, radius * z1};

            mesh.push_back({p1, p2, p3});
            mesh.push_back({p1, p3, p4});
        }
    }
    return mesh;
}


struct Model {
    std::vector<Triangle> mesh;
    Vec3 position;      // Позиция в мире
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float rotSpeedX = 0.0f;
    float rotSpeedY = 0.0f;
};

// Перемещение (сдвиг) вектора
Vec3 translate(Vec3 p, Vec3 offset) {
    return { p.x + offset.x, p.y + offset.y, p.z + offset.z };
}

int main() {
    // 1. Создаем объекты сцены
    std::vector<Model> scene;

    Model torus = { generateTorus(1.0f, 0.4f, 15, 30), {0.0f, 0.0f, 0.0f}, 0, 0, 0.02f, 0.04f };
    Model cube = { generateCube(1.2f), {-2.5f, 0.0f, 0.0f}, 0, 0, 0.03f, 0.02f };
    Model sphere = { generateSphere(0.8f, 12, 24), {2.5f, 0.0f, 0.0f}, 0, 0, 0.01f, 0.05f };

    scene.push_back(torus);
    scene.push_back(cube);
    scene.push_back(sphere);

    // Параметры камеры
    float cameraOrbitAngle = 0.0f;
    float cameraRadius = 7.0f; // Дистанция от центра сцены
    float zoomScale = 60.0f;

    Vec3 lightDir = {0.0f, -0.5f, -1.0f};
    lightDir.normalize();

    std::cout << "\x1b[2J"; // Очистка экрана

    while (true) {
        std::vector<char> screenBuffer(WIDTH * HEIGHT, ' ');
        std::vector<float> zBuffer(WIDTH * HEIGHT, FLT_MAX);

        // Вращаем камеру по орбите
        cameraOrbitAngle += 0.02f; 

        // Перебираем все объекты на сцене
        for (auto& model : scene) {
            // Обновляем собственное вращение объекта
            model.rotationX += model.rotSpeedX;
            model.rotationY += model.rotSpeedY;

            for (const auto& tri : model.mesh) {
                Triangle transformed;

                for (int i = 0; i < 3; ++i) {
                    Vec3 p = tri.p[i];
                    
                    // А) Локальное вращение объекта вокруг своего центра
                    p = rotateX(p, model.rotationX);
                    p = rotateY(p, model.rotationY);
                    
                    // Б) Мировое смещение (ставим объект на его место в сцене)
                    p = translate(p, model.position);

                    // В) Трансформация Камеры (Вращаем сцену вокруг центра в обратную сторону)
                    // Это заставляет камеру летать вокруг центра сцены
                    p = rotateY(p, -cameraOrbitAngle); 
                    
                    // Сдвигаем сцену назад относительно камеры
                    p.z += cameraRadius; 

                    transformed.p[i] = p;
                }

                // Расчет освещения
                Vec3 edge1 = transformed.p[1] - transformed.p[0];
                Vec3 edge2 = transformed.p[2] - transformed.p[0];
                Vec3 normal = edge1.cross(edge2);
                normal.normalize();

                float intensity = std::abs(normal.dot(lightDir));
                char symbol = getAsciiChar(intensity);

                // Проекция на экран
                Triangle projected;
                for (int i = 0; i < 3; ++i) {
                    projected.p[i] = project(transformed.p[i], zoomScale);
                }

                // Растеризация в общий буфер
                rasterizeTriangle(projected, symbol, screenBuffer, zBuffer);
            }
        }

        // Вывод буфера на экран
        std::cout << "\x1b[H";
        std::string output = "";
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                output += screenBuffer[y * WIDTH + x];
            }
            output += '\n';
        }
        std::cout << output;

        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
    return 0;
}
