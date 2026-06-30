// #include <iostream>
// #include <vector>
// #include <cmath>
// #include <algorithm>
// #include <cstdint>
// #include <fstream>
// #include <sstream>
// #include <string>
//
// // Жесткие габариты ОДНОГО ракурса в символах под твой интерфейс
// const int VIEW_CHARS_W = 45;
// const int VIEW_CHARS_H = 20;
//
// // Размеры всей сетки 2х2 в символах (с учетом тонких рамок-разделителей)
// const int TOTAL_CHARS_W = VIEW_CHARS_W * 2 + 3;
// const int TOTAL_CHARS_H = VIEW_CHARS_H * 2 + 3;
//
// // Внутреннее графическое разрешение одного ракурса (благодаря матрице блоков 2х2)
// const int BIT_W = VIEW_CHARS_W * 2; // 30 точек
// const int BIT_H = VIEW_CHARS_H * 2; // 60 точек
//
// // Пропорции 3D-масштабирования под сетку блоков 2х2
// const float SCALE_X = BIT_W * 0.45f;
// const float SCALE_Y = BIT_H * 0.45f;
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
//     Vec3 faceNormal;
//     bool isFrontFacing;
// };
//
// struct Edge {
//     int v1, v2;
//     int tri1 = -1;
//     int tri2 = -1;
// };
//
// struct ViewAngle {
//     std::string name;
//     float rotX;
//     float rotY;
// };
//
// void drawSubpixelLine(std::vector<std::vector<bool>>& bitGrid, int x0, int y0, int x1, int y1) {
//     int dx = std::abs(x1 - x0);
//     int dy = std::abs(y1 - y0);
//     int sx = (x0 < x1) ? 1 : -1;
//     int sy = (y0 < y1) ? 1 : -1;
//     int err = dx - dy;
//
//     while (true) {
//         if (x0 >= 0 && x0 < BIT_W && y0 >= 0 && y0 < BIT_H) {
//             bitGrid[y0][x0] = true;
//         }
//         if (x0 == x1 && y0 == y1) break;
//         int e2 = 2 * err;
//         if (e2 > -dy) { err -= dy; x0 += sx; }
//         if (e2 < dx)  { err += dx; y0 += sy; }
//     }
// }
//
// // Декодер микро-матрицы 2х2 в один сплошной Юникод-блок
// std::string encodeBlockChar(const std::vector<std::vector<bool>>& bitGrid, int startX, int startY) {
//     int mask = 0;
//     if (bitGrid[startY + 0][startX + 0]) mask |= 1; // Топ-лево
//     if (bitGrid[startY + 0][startX + 1]) mask |= 2; // Топ-право
//     if (bitGrid[startY + 1][startX + 0]) mask |= 4; // Бот-лево
//     if (bitGrid[startY + 1][startX + 1]) mask |= 8; // Бот-право
//
//     // Таблица компактных псевдографических блоков 2х2
//     switch (mask) {
//         case 0:  return " ";
//         case 1:  return "▘";
//         case 2:  return "▝";
//         case 3:  return "▀";
//         case 4:  return "▖";
//         case 5:  return "▌";
//         case 6:  return "▞";
//         case 7:  return "▛";
//         case 8:  return "▕";
//         case 9:  return "▚";
//         case 10: return "▐";
//         case 11: return "▜";
//         case 12: return "▄";
//         case 13: return "▙";
//         case 14: return "▟";
//         case 15: return "■"; // Полный заполненный блок (или "█")
//     }
//     return " ";
// }
//
// std::vector<Triangle> loadOBJ(const std::string& filename, std::vector<Vec3>& vertices) {
//     std::vector<Triangle> triangles;
//     std::ifstream file(filename);
//     if (!file.is_open()) return triangles;
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
//             std::vector<std::string> tokens;
//             std::string token;
//             while (ss >> token) tokens.push_back(token);
//             auto get_idx = [](const std::string& s) { return std::stoi(s.substr(0, s.find('/'))) - 1; };
//
//             if (tokens.size() >= 3) {
//                 int idx0 = get_idx(tokens[0]);
//                 int idx1 = get_idx(tokens[1]);
//                 int idx2 = get_idx(tokens[2]);
//                 triangles.push_back({{idx0, idx1, idx2}, {0,0,0}, false});
//                 if (tokens.size() == 4) {
//                     int idx3 = get_idx(tokens[3]);
//                     triangles.push_back({{idx0, idx2, idx3}, {0,0,0}, false});
//                 }
//             }
//         }
//     }
//     return triangles;
// }
//
// int main() {
//     std::vector<Vec3> baseVertices;
//     std::vector<Triangle> triangles = loadOBJ("moto_simple_1.obj", baseVertices);
//     if (triangles.empty()) {
//         std::cerr << "Положите .obj файл модели рядом с бинарником!" << std::endl;
//         return 1;
//     }
//
//     std::vector<Edge> edges;
//     auto addEdge = [&](int v1, int v2, int triIdx) {
//         if (v1 > v2) std::swap(v1, v2);
//         for (auto& e : edges) {
//             if (e.v1 == v1 && e.v2 == v2) { e.tri2 = triIdx; return; }
//         }
//         edges.push_back({v1, v2, triIdx, -1});
//     };
//     for (size_t i = 0; i < triangles.size(); ++i) {
//         addEdge(triangles[i].vIdx[0], triangles[i].vIdx[1], i);
//         addEdge(triangles[i].vIdx[1], triangles[i].vIdx[2], i);
//         addEdge(triangles[i].vIdx[2], triangles[i].vIdx[0], i);
//     }
//
//     std::vector<ViewAngle> views = {
//         {"FRONT",  0.0f,    0.0f},
//         {"BACK",   0.0f,    3.14159f},
//         {"LEFT",   0.0f,    4.71239f},
//         {"RIGHT",  0.0f,    1.5708f}
//     };
//
//     std::vector<std::vector<std::string>> totalConsoleGrid(TOTAL_CHARS_H, std::vector<std::string>(TOTAL_CHARS_W, " "));
//
//     // Строим тонкую рамку интерфейса вокруг окон 15х30
//     for (int x = 0; x < TOTAL_CHARS_W; ++x) {
//         totalConsoleGrid[0][x] = "─";
//         totalConsoleGrid[VIEW_CHARS_H + 1][x] = "─";
//         totalConsoleGrid[TOTAL_CHARS_H - 1][x] = "─";
//     }
//     for (int y = 0; y < TOTAL_CHARS_H; ++y) {
//         totalConsoleGrid[y][0] = "│";
//         totalConsoleGrid[y][VIEW_CHARS_W + 1] = "│";
//         totalConsoleGrid[y][TOTAL_CHARS_W - 1] = "│";
//     }
//     totalConsoleGrid[0][0] = "┌"; totalConsoleGrid[0][TOTAL_CHARS_W - 1] = "┐";
//     totalConsoleGrid[TOTAL_CHARS_H - 1][0] = "└"; totalConsoleGrid[TOTAL_CHARS_H - 1][TOTAL_CHARS_W - 1] = "┘";
//     totalConsoleGrid[VIEW_CHARS_H + 1][0] = "├"; totalConsoleGrid[VIEW_CHARS_H + 1][TOTAL_CHARS_W - 1] = "┤";
//     totalConsoleGrid[0][VIEW_CHARS_W + 1] = "┬"; totalConsoleGrid[TOTAL_CHARS_H - 1][VIEW_CHARS_W + 1] = "┴";
//     totalConsoleGrid[VIEW_CHARS_H + 1][VIEW_CHARS_W + 1] = "┼";
//
//     for (int vIdx = 0; vIdx < 4; ++vIdx) {
//         const auto& view = views[vIdx];
//
//         std::vector<Vec3> transVertices(baseVertices.size());
//         std::vector<std::pair<int, int>> subpixelCoords(baseVertices.size());
//
//         for (size_t i = 0; i < baseVertices.size(); ++i) {
//             Vec3 v = baseVertices[i];
//             float x1 = v.x * cosf(view.rotY) + v.z * sinf(view.rotY);
//             float z1 = -v.x * sinf(view.rotY) + v.z * cosf(view.rotY);
//             float y2 = v.y * cosf(view.rotX) - z1 * sinf(view.rotX);
//             float z2 = v.y * sinf(view.rotX) + z1 * cosf(view.rotX);
//
//             transVertices[i] = {x1, y2, z2};
//
//             subpixelCoords[i] = {
//                 static_cast<int>(x1 * SCALE_X + BIT_W / 2),
//                 static_cast<int>(-y2 * SCALE_Y + BIT_H / 2)
//             };
//         }
//
//         for (auto& t : triangles) {
//             Vec3 e1 = transVertices[t.vIdx[1]] - transVertices[t.vIdx[0]];
//             Vec3 e2 = transVertices[t.vIdx[2]] - transVertices[t.vIdx[0]];
//             t.faceNormal = e1.cross(e2).normalize();
//             t.isFrontFacing = (t.faceNormal.z > 0.0f);
//         }
//
//         // Для сохранения качества на таком микро-размере Z-буфер рассчитываем строго попиксельно
//         std::vector<float> zBuffer(BIT_W * BIT_H, -1e9f);
//         for (const auto& t : triangles) {
//             if (!t.isFrontFacing) continue;
//
//             int x0 = subpixelCoords[t.vIdx[0]].first, y0 = subpixelCoords[t.vIdx[0]].second;
//             int x1 = subpixelCoords[t.vIdx[1]].first, y1 = subpixelCoords[t.vIdx[1]].second;
//             int x2 = subpixelCoords[t.vIdx[2]].first, y2 = subpixelCoords[t.vIdx[2]].second;
//
//             int minX = std::max(0, std::min({x0, x1, x2}));
//             int maxX = std::min(BIT_W - 1, std::max({x0, x1, x2}));
//             int minY = std::max(0, std::min({y0, y1, y2}));
//             int maxY = std::min(BIT_H - 1, std::max({y0, y1, y2}));
//
//             float d = static_cast<float>((y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2));
//             if (std::abs(d) < 1e-5f) continue;
//
//             for (int py = minY; py <= maxY; ++py) {
//                 for (int px = minX; px <= maxX; ++px) {
//                     float w0 = ((y1 - y2) * (px - x2) + (x2 - x1) * (py - y2)) / d;
//                     float w1 = ((y2 - y0) * (px - x2) + (x0 - x2) * (py - y2)) / d;
//                     float w2 = 1.0f - w0 - w1;
//
//                     if (w0 >= -1e-3f && w1 >= -1e-3f && w2 >= -1e-3f) {
//                         float z = w0 * transVertices[t.vIdx[0]].z +
//                         w1 * transVertices[t.vIdx[1]].z +
//                         w2 * transVertices[t.vIdx[2]].z;
//                         int idx = py * BIT_W + px;
//                         if (z > zBuffer[idx]) zBuffer[idx] = z;
//                     }
//                 }
//             }
//         }
//
//         std::vector<std::vector<bool>> bitGrid(BIT_H, std::vector<bool>(BIT_W, false));
//         const float CONTINUITY_THRESHOLD = 0.20f; // Оптимально для микро-размера
//
//         for (const auto& e : edges) {
//             bool isSilhouette = false;
//             bool isCrease = false;
//
//             bool f1 = (e.tri1 != -1) ? triangles[e.tri1].isFrontFacing : false;
//             bool f2 = (e.tri2 != -1) ? triangles[e.tri2].isFrontFacing : false;
//
//             if (e.tri2 == -1) isSilhouette = f1;
//             else if (f1 != f2) isSilhouette = true;
//
//             if (!isSilhouette && e.tri1 != -1 && e.tri2 != -1 && f1 && f2) {
//                 float dotNormal = triangles[e.tri1].faceNormal.dot(triangles[e.tri2].faceNormal);
//                 if (dotNormal < 0.20f) isCrease = true;
//             }
//
//             if (isSilhouette || isCrease) {
//                 int x0 = subpixelCoords[e.v1].first, y0 = subpixelCoords[e.v1].second;
//                 int x1 = subpixelCoords[e.v2].first, y1 = subpixelCoords[e.v2].second;
//                 float z0 = transVertices[e.v1].z;
//                 float z1 = transVertices[e.v2].z;
//
//                 const int steps = 12;
//                 int visiblePoints = 0;
//
//                 for (int s = 0; s <= steps; ++s) {
//                     float t = (float)s / steps;
//                     int currX = static_cast<int>(x0 + (x1 - x0) * t);
//                     int currY = static_cast<int>(y0 + (y1 - y0) * t);
//                     float currZ = z0 + (z1 - z0) * t;
//
//                     if (currX >= 0 && currX < BIT_W && currY >= 0 && currY < BIT_H) {
//                         int bufferIdx = currY * BIT_W + currX;
//                         // Слегка увеличенный bias (0.025f), чтобы на низком разрешении линии не съедались
//                         if (zBuffer[bufferIdx] <= currZ + 0.025f) visiblePoints++;
//                     }
//                 }
//
//                 float visibilityRate = static_cast<float>(visiblePoints) / (steps + 1);
//
//                 if (visibilityRate >= CONTINUITY_THRESHOLD) {
//                     drawSubpixelLine(bitGrid, x0, y0, x1, y1);
//                 }
//             }
//         }
//
//         int charOffsetX = (vIdx % 2 == 0) ? 1 : VIEW_CHARS_W + 2;
//         int charOffsetY = (vIdx / 2 == 0) ? 1 : VIEW_CHARS_H + 2;
//
//         // Кодируем графические субпиксели 2х2 в ASCII-блоки
//         for (int cy = 0; cy < VIEW_CHARS_H; ++cy) {
//             for (int cx = 0; cx < VIEW_CHARS_W; ++cx) {
//                 totalConsoleGrid[charOffsetY + cy][charOffsetX + cx] = encodeBlockChar(bitGrid, cx * 2, cy * 2);
//             }
//         }
//
//         // Микро-тег для индикации ракурса
//         std::string label = view.name.substr(0, 2); // Берем первые 2 буквы (FR, BA, LE, RI)
//         totalConsoleGrid[charOffsetY][charOffsetX] = label.substr(0, 1);
//         totalConsoleGrid[charOffsetY][charOffsetX + 1] = label.substr(1, 1);
//     }
//
//     // Сохраняем финальный компактный текстовый интерфейс
//     std::ofstream outFile("moto_micro_interface_lineart.txt");
//     for (const auto& row : totalConsoleGrid) {
//         for (const auto& symbol : row) outFile << symbol;
//         outFile << "\n";
//     }
//     outFile.close();
//
//     std::cout << "[УСПЕХ] Создан микро-интерфейс! Результат в 'micro_interface_lineart.txt'" << std::endl;
//     return 0;
// }


#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

// Размеры ОДНОГО ракурса в символах (30 строк, 15 колонок)
const int VIEW_CHARS_W = 100;
const int VIEW_CHARS_H = 35;

// Размеры всей сетки 2х2 в символах (включая рамки)
const int TOTAL_CHARS_W = VIEW_CHARS_W * 2 + 3;
const int TOTAL_CHARS_H = VIEW_CHARS_H * 2 + 3;

// Графическое разрешение одной мини-матрицы (1 точка в ширину, 2 в высоту на символ)
const int BIT_W = VIEW_CHARS_W;      // 15 точек в ширину
const int BIT_H = VIEW_CHARS_H * 2;  // 60 точек в высоту

// ГЛОБАЛЬНЫЙ МАСШТАБ (ОТДАЛЕНИЕ КАМЕРЫ)
// Коэффициент уменьшен с 0.45f до 0.35f, чтобы модель полностью входила в 15x60 пикселей
const float SCALE_X = BIT_W * 0.45f;
const float SCALE_Y = BIT_H * 0.45f;

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

void drawSubpixelLine(std::vector<std::vector<bool>>& bitGrid, int x0, int y0, int x1, int y1) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (x0 >= 0 && x0 < BIT_W && y0 >= 0 && y0 < BIT_H) {
            bitGrid[y0][x0] = true;
        }
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx)  { err += dx; y0 += sy; }
    }
}

std::string encodeHalfBlockChar(const std::vector<std::vector<bool>>& bitGrid, int cx, int cy) {
    bool top = bitGrid[cy * 2 + 0][cx];
    bool bot = bitGrid[cy * 2 + 1][cx];

    if (top && bot) return "█";
    if (top)        return "▀";
    if (bot)        return "▄";
    return " ";
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
        std::cerr << "Положите suzanne.obj рядом с бинарником!" << std::endl;
        return 1;
    }

    // Авто-центрирование модели (смещение геометрии в 0,0,0 перед рендером)
    Vec3 minV = {1e9f, 1e9f, 1e9f}, maxV = {-1e9f, -1e9f, -1e9f};
    for (const auto& v : baseVertices) {
        minV.x = std::min(minV.x, v.x); minV.y = std::min(minV.y, v.y); minV.z = std::min(minV.z, v.z);
        maxV.x = std::max(maxV.x, v.x); maxV.y = std::max(maxV.y, v.y); maxV.z = std::max(maxV.z, v.z);
    }
    Vec3 center = (minV + maxV) * 0.5f;
    for (auto& v : baseVertices) {
        v = v - center;
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
        {"FRONT",  0.0f,    0.0f},
        {"BACK",   0.0f,    3.14159f},
        {"LEFT",   0.0f,    4.71239f},
        {"RIGHT",  0.0f,    1.5708f}
    };

    std::vector<std::vector<std::string>> totalConsoleGrid(TOTAL_CHARS_H, std::vector<std::string>(TOTAL_CHARS_W, " "));

    // Базовая сетка рамок
    for (int x = 0; x < TOTAL_CHARS_W; ++x) {
        totalConsoleGrid[0][x] = "─";
        totalConsoleGrid[VIEW_CHARS_H + 1][x] = "─";
        totalConsoleGrid[TOTAL_CHARS_H - 1][x] = "─";
    }
    for (int y = 0; y < TOTAL_CHARS_H; ++y) {
        totalConsoleGrid[y][0] = "│";
        totalConsoleGrid[y][VIEW_CHARS_W + 1] = "│";
        totalConsoleGrid[y][TOTAL_CHARS_W - 1] = "│";
    }
    totalConsoleGrid[0][0] = "┌"; totalConsoleGrid[0][TOTAL_CHARS_W - 1] = "┐";
    totalConsoleGrid[TOTAL_CHARS_H - 1][0] = "└"; totalConsoleGrid[TOTAL_CHARS_H - 1][TOTAL_CHARS_W - 1] = "┘";
    totalConsoleGrid[VIEW_CHARS_H + 1][0] = "├"; totalConsoleGrid[VIEW_CHARS_H + 1][TOTAL_CHARS_W - 1] = "┤";
    totalConsoleGrid[0][VIEW_CHARS_W + 1] = "┬"; totalConsoleGrid[TOTAL_CHARS_H - 1][VIEW_CHARS_W + 1] = "┴";
    totalConsoleGrid[VIEW_CHARS_H + 1][VIEW_CHARS_W + 1] = "┼";

    // ВСТРАИВАЕМ МЕТКИ ПРЯМО В РАМКИ (чтобы разгрузить графическое поле)
    totalConsoleGrid[0][2] = "F"; totalConsoleGrid[0][3] = "R";
    totalConsoleGrid[0][VIEW_CHARS_W + 3] = "B"; totalConsoleGrid[0][VIEW_CHARS_W + 4] = "A";
    totalConsoleGrid[VIEW_CHARS_H + 1][2] = "L"; totalConsoleGrid[VIEW_CHARS_H + 1][3] = "E";
    totalConsoleGrid[VIEW_CHARS_H + 1][VIEW_CHARS_W + 3] = "R"; totalConsoleGrid[VIEW_CHARS_H + 1][VIEW_CHARS_W + 4] = "I";

    for (int vIdx = 0; vIdx < 4; ++vIdx) {
        const auto& view = views[vIdx];

        std::vector<Vec3> transVertices(baseVertices.size());
        std::vector<std::pair<int, int>> subpixelCoords(baseVertices.size());

        for (size_t i = 0; i < baseVertices.size(); ++i) {
            Vec3 v = baseVertices[i];
            float x1 = v.x * cosf(view.rotY) + v.z * sinf(view.rotY);
            float z1 = -v.x * sinf(view.rotY) + v.z * cosf(view.rotY);
            float y2 = v.y * cosf(view.rotX) - z1 * sinf(view.rotX);
            float z2 = v.y * sinf(view.rotX) + z1 * cosf(view.rotX);

            transVertices[i] = {x1, y2, z2};

            subpixelCoords[i] = {
                static_cast<int>(x1 * SCALE_X + BIT_W / 2),
                static_cast<int>(-y2 * SCALE_Y + BIT_H / 2)
            };
        }

        for (auto& t : triangles) {
            Vec3 e1 = transVertices[t.vIdx[1]] - transVertices[t.vIdx[0]];
            Vec3 e2 = transVertices[t.vIdx[2]] - transVertices[t.vIdx[0]];
            t.faceNormal = e1.cross(e2).normalize();
            t.isFrontFacing = (t.faceNormal.z > 0.0f);
        }

        std::vector<float> zBuffer(BIT_W * BIT_H, -1e9f);
        for (const auto& t : triangles) {
            if (!t.isFrontFacing) continue;

            int x0 = subpixelCoords[t.vIdx[0]].first, y0 = subpixelCoords[t.vIdx[0]].second;
            int x1 = subpixelCoords[t.vIdx[1]].first, y1 = subpixelCoords[t.vIdx[1]].second;
            int x2 = subpixelCoords[t.vIdx[2]].first, y2 = subpixelCoords[t.vIdx[2]].second;

            int minX = std::max(0, std::min({x0, x1, x2}));
            int maxX = std::min(BIT_W - 1, std::max({x0, x1, x2}));
            int minY = std::max(0, std::min({y0, y1, y2}));
            int maxY = std::min(BIT_H - 1, std::max({y0, y1, y2}));

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
                        int idx = py * BIT_W + px;
                        if (z > zBuffer[idx]) zBuffer[idx] = z;
                    }
                }
            }
        }

        std::vector<std::vector<bool>> bitGrid(BIT_H, std::vector<bool>(BIT_W, false));
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
                if (dotNormal < 0.25f) isCrease = true;
            }

            if (isSilhouette || isCrease) {
                int x0 = subpixelCoords[e.v1].first, y0 = subpixelCoords[e.v1].second;
                int x1 = subpixelCoords[e.v2].first, y1 = subpixelCoords[e.v2].second;
                float z0 = transVertices[e.v1].z;
                float z1 = transVertices[e.v2].z;

                const int steps = 10;
                int visiblePoints = 0;

                for (int s = 0; s <= steps; ++s) {
                    float t = (float)s / steps;
                    int currX = static_cast<int>(x0 + (x1 - x0) * t);
                    int currY = static_cast<int>(y0 + (y1 - y0) * t);
                    float currZ = z0 + (z1 - z0) * t;

                    if (currX >= 0 && currX < BIT_W && currY >= 0 && currY < BIT_H) {
                        int bufferIdx = currY * BIT_W + currX;
                        if (zBuffer[bufferIdx] <= currZ + 0.03f) visiblePoints++;
                    }
                }

                float visibilityRate = static_cast<float>(visiblePoints) / (steps + 1);

                if (visibilityRate >= CONTINUITY_THRESHOLD) {
                    drawSubpixelLine(bitGrid, x0, y0, x1, y1);
                }
            }
        }

        int charOffsetX = (vIdx % 2 == 0) ? 1 : VIEW_CHARS_W + 2;
        int charOffsetY = (vIdx / 2 == 0) ? 1 : VIEW_CHARS_H + 2;

        for (int cy = 0; cy < VIEW_CHARS_H; ++cy) {
            for (int cx = 0; cx < VIEW_CHARS_W; ++cx) {
                totalConsoleGrid[charOffsetY + cy][charOffsetX + cx] = encodeHalfBlockChar(bitGrid, cx, cy);
            }
        }
    }

    std::ofstream outFile("quad_20x45_distanced.txt");
    for (const auto& row : totalConsoleGrid) {
        for (const auto& symbol : row) outFile << symbol;
        outFile << "\n";
    }
    outFile.close();

    std::cout << "[ГОТОВО] Модель отдалена и центрирована. Результат: 'quad_15x30_distanced.txt'" << std::endl;
    return 0;
}
