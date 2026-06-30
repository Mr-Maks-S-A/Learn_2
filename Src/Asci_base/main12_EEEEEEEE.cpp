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
    std::vector<Triangle> triangles = loadOBJ("moto_simple_1.obj", baseVertices);
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

        // 3. Анализ ребер и отрисовка с контролем непрерывности
        std::vector<uint8_t> image(WIDTH * HEIGHT * 4, 255);

        // ПАРАМЕТР НЕПРЕРЫВНОСТИ: какая доля точек ребра должна быть видима,
        // чтобы мы нарисовали всё ребро целиком (0.7 = 70%).
        // Снижение этого параметра (например, до 0.5) делает линии более цельными.
        const float CONTINUITY_THRESHOLD = 0.1f;

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

            if (!isSilhouette && e.tri1 != -1 && e.tri2 != -1 && f1 && f2) {
                float dotNormal = triangles[e.tri1].faceNormal.dot(triangles[e.tri2].faceNormal);
                if (dotNormal < 0.17f) {
                    isCrease = true;
                }
            }

            if (isSilhouette || isCrease) {
                int x0 = screenCoords[e.v1].first, y0 = screenCoords[e.v1].second;
                int x1 = screenCoords[e.v2].first, y1 = screenCoords[e.v2].second;
                float z0 = transVertices[e.v1].z;
                float z1 = transVertices[e.v2].z;

                // Считаем видимость ребра по точкам
                const int steps = 16; // Немного увеличили шаг для точности контроля
                int visiblePoints = 0;

                for (int s = 0; s <= steps; ++s) {
                    float t = (float)s / steps;
                    int currX = static_cast<int>(x0 + (x1 - x0) * t);
                    int currY = static_cast<int>(y0 + (y1 - y0) * t);
                    float currZ = z0 + (z1 - z0) * t;

                    if (currX >= 0 && currX < WIDTH && currY >= 0 && currY < HEIGHT) {
                        int bufferIdx = currY * WIDTH + currX;

                        // Если точка ближе или находится в пределах небольшого bias, она видима
                        if (zBuffer[bufferIdx] <= currZ + 0.008f) {
                            visiblePoints++;
                        }
                    } else {
                        // Если точка улетела за экран, не считаем её видимой
                    }
                }

                // Вычисляем коэффициент непрерывности для текущего ребра
                float visibilityRate = static_cast<float>(visiblePoints) / (steps + 1);

                // Отрисовываем ребро, если оно преодолело порог непрерывности
                if (visibilityRate >= CONTINUITY_THRESHOLD) {
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
}*/





#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

// Размеры текстового экрана (мини-матрица ячеек)
const int ASCII_WIDTH = 120;
const int ASCII_HEIGHT = 50;

// Масштаб для подгонки 3D модели под размер текстового окна
const float ASCII_SCALE_X = 50.0f;
const float ASCII_SCALE_Y = 22.0f; // Коэффициент Y меньше, так как текстовые символы вытянуты по вертикали

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

// Функция отрисовки символьной линии с паттерн-матчингом направления (как в Stone Story)
void drawAsciiLine(std::vector<std::string>& grid, int x0, int y0, int x1, int y1) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    // Вычисляем базовый наклон всей линии для выбора символа
    char edgeChar = '-';
    if (dx == 0) {
        edgeChar = '|';
    } else {
        float slope = static_cast<float>(y1 - y0) / static_cast<float>(x1 - x0);
        // Инвертируем наклон, так как в консоли Y идет сверху вниз
        slope = -slope;

        if (std::abs(slope) > 2.0f) edgeChar = '|';
        else if (std::abs(slope) < 0.4f) edgeChar = '-';
        else if (slope >= 0.4f && slope <= 2.0f) edgeChar = '/';
        else edgeChar = '\\';
    }

    while (true) {
        if (x0 >= 0 && x0 < ASCII_WIDTH && y0 >= 0 && y0 < ASCII_HEIGHT) {
            char current = grid[y0][x0];

            // Наложение символов (Resolving конфликтов ячеек)
            if (current == ' ') {
                grid[y0][x0] = edgeChar;
            } else if ((current == '-' && edgeChar == '|') || (current == '|' && edgeChar == '-')) {
                grid[y0][x0] = '+';
            } else if ((current == '/' && edgeChar == '\\') || (current == '\\' && edgeChar == '/')) {
                grid[y0][x0] = 'X';
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
            std::vector<std::string> tokens;
            std::string token;
            while (ss >> token) tokens.push_back(token);
            auto get_idx = [](const std::string& s) { return std::stoi(s.substr(0, s.find('/'))) - 1; };

            if (tokens.size() >= 3) {
                int idx0 = get_idx(tokens[0]);
                int idx1 = get_idx(tokens[1]);
                int idx2 = get_idx(tokens[2]);
                triangles.push_back({{idx0, idx1, idx2}, {0,0,0}, false});
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
        std::cerr << "Положите файл куба/обезьяны/мотоцикла рядом с бинарником!" << std::endl;
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

    // Для субпиксельной точности Z-буфер рассчитываем в высоком разрешении (1000x1000)
    const int RENDER_W = 1000;
    const int RENDER_H = 1000;
    const float RENDER_SCALE = 400.0f;

    for (const auto& view : views) {
        std::vector<Vec3> transVertices(baseVertices.size());
        std::vector<std::pair<int, int>> highResCoords(baseVertices.size());
        std::vector<std::pair<int, int>> asciiCoords(baseVertices.size());

        for (size_t i = 0; i < baseVertices.size(); ++i) {
            Vec3 v = baseVertices[i];
            float x1 = v.x * cosf(view.rotY) + v.z * sinf(view.rotY);
            float z1 = -v.x * sinf(view.rotY) + v.z * cosf(view.rotY);
            float y2 = v.y * cosf(view.rotX) - z1 * sinf(view.rotX);
            float z2 = v.y * sinf(view.rotX) + z1 * cosf(view.rotX);

            transVertices[i] = {x1, y2, z2};

            // Координаты для точного Z-буфера
            highResCoords[i] = {
                static_cast<int>(x1 * RENDER_SCALE + RENDER_W / 2),
                static_cast<int>(-y2 * RENDER_SCALE + RENDER_H / 2)
            };

            // Координаты низкого разрешения для ASCII-сетки
            asciiCoords[i] = {
                static_cast<int>(x1 * ASCII_SCALE_X + ASCII_WIDTH / 2),
                static_cast<int>(-y2 * ASCII_SCALE_Y + ASCII_HEIGHT / 2)
            };
        }

        for (auto& t : triangles) {
            Vec3 e1 = transVertices[t.vIdx[1]] - transVertices[t.vIdx[0]];
            Vec3 e2 = transVertices[t.vIdx[2]] - transVertices[t.vIdx[0]];
            t.faceNormal = e1.cross(e2).normalize();
            t.isFrontFacing = (t.faceNormal.z > 0.0f);
        }

        // Рендерим точный Z-буфер
        std::vector<float> zBuffer(RENDER_W * RENDER_H, -1e9f);
        for (const auto& t : triangles) {
            if (!t.isFrontFacing) continue;

            int x0 = highResCoords[t.vIdx[0]].first, y0 = highResCoords[t.vIdx[0]].second;
            int x1 = highResCoords[t.vIdx[1]].first, y1 = highResCoords[t.vIdx[1]].second;
            int x2 = highResCoords[t.vIdx[2]].first, y2 = highResCoords[t.vIdx[2]].second;

            int minX = std::max(0, std::min({x0, x1, x2}));
            int maxX = std::min(RENDER_W - 1, std::max({x0, x1, x2}));
            int minY = std::max(0, std::min({y0, y1, y2}));
            int maxY = std::min(RENDER_H - 1, std::max({y0, y1, y2}));

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
                        int idx = py * RENDER_W + px;
                        if (z > zBuffer[idx]) zBuffer[idx] = z;
                    }
                }
            }
        }

        // Текстовая ASCII матрица кадра
        std::vector<std::string> asciiGrid(ASCII_HEIGHT, std::string(ASCII_WIDTH, ' '));
        const float CONTINUITY_THRESHOLD = 0.15f;

        for (const auto& e : edges) {
            bool isSilhouette = false;
            bool isCrease = false;

            bool f1 = (e.tri1 != -1) ? triangles[e.tri1].isFrontFacing : false;
            bool f2 = (e.tri2 != -1) ? triangles[e.tri2].isFrontFacing : false;

            if (e.tri2 == -1) isSilhouette = f1;
            else if (f1 != f2) isSilhouette = true;

            if (!isSilhouette && e.tri1 != -1 && e.tri2 != -1 && f1 && f2) {
                float dotNormal = triangles[e.tri1].faceNormal.dot(triangles[e.tri2].faceNormal);
                if (dotNormal < 0.25f) isCrease = true; // Угол граней
            }

            if (isSilhouette || isCrease) {
                // Используем координаты высокого разрешения для проверки Z-видимости
                int hX0 = highResCoords[e.v1].first, hY0 = highResCoords[e.v1].second;
                int hX1 = highResCoords[e.v2].first, hY1 = highResCoords[e.v2].second;
                float z0 = transVertices[e.v1].z;
                float z1 = transVertices[e.v2].z;

                const int steps = 16;
                int visiblePoints = 0;

                for (int s = 0; s <= steps; ++s) {
                    float t = (float)s / steps;
                    int currX = static_cast<int>(hX0 + (hX1 - hX0) * t);
                    int currY = static_cast<int>(hY0 + (hY1 - hY0) * t);
                    float currZ = z0 + (z1 - z0) * t;

                    if (currX >= 0 && currX < RENDER_W && currY >= 0 && currY < RENDER_H) {
                        int bufferIdx = currY * RENDER_W + currX;
                        if (zBuffer[bufferIdx] <= currZ + 0.008f) visiblePoints++;
                    }
                }

                float visibilityRate = static_cast<float>(visiblePoints) / (steps + 1);

                // Если ребро прошло Z-тест, рисуем его направленными ASCII символами
                if (visibilityRate >= CONTINUITY_THRESHOLD) {
                    int aX0 = asciiCoords[e.v1].first, aY0 = asciiCoords[e.v1].second;
                    int aX1 = asciiCoords[e.v2].first, aY1 = asciiCoords[e.v2].second;

                    drawAsciiLine(asciiGrid, aX0, aY0, aX1, aY1);
                }
            }
        }

        // Сохраняем текстовый файл
        std::string filename = "ascii_lineart_" + view.name + ".txt";
        std::ofstream outFile(filename);
        for (const auto& row : asciiGrid) {
            outFile << row << "\n";
        }
        outFile.close();
        std::cout << "[ASCII] Успешно сохранен текстовый файл: " << filename << std::endl;
    }

    return 0;
}
