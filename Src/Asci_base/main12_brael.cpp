#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

// Размеры ОДНОЙ субпиксельной матрицы (в символах)
// 50x35 символов Брайля дают внутреннее графическое разрешение 100x140 точек на один ракурс!
const int VIEW_CHARS_W = 50;
const int VIEW_CHARS_H = 35;

// Общие размеры консольного экрана для сетки 2х2 (в символах)
const int TOTAL_CHARS_W = VIEW_CHARS_W * 2 + 4; // +4 для рамок-разделителей
const int TOTAL_CHARS_H = VIEW_CHARS_H * 2 + 3;

// Коэффициенты масштабирования 3D-пространства под субпиксельное разрешение Брайля (2 точки в ширину, 4 в высоту на символ)
const float SCALE_X = (VIEW_CHARS_W * 2) * 0.45f;
const float SCALE_Y = (VIEW_CHARS_H * 4) * 0.45f;

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

// Функция отрисовки линии Брезенхэма в битовую маску субпикселей
void drawSubpixelLine(std::vector<std::vector<bool>>& bitGrid, int x0, int y0, int x1, int y1, int w, int h) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (x0 >= 0 && x0 < w && y0 >= 0 && y0 < h) {
            bitGrid[y0][x0] = true;
        }
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx)  { err += dx; y0 += sy; }
    }
}

// Конвертер мини-матрицы 2х4 бита в UTF-8 символ Брайля
std::string encodeBrailleChar(const std::vector<std::vector<bool>>& bitGrid, int startX, int startY) {
    int code = 0;
    // Карта смещений точек Брайля в стандартном Юникоде:
    // По левой стороне (сверху вниз): биты 0, 1, 2, 6
    // По правой стороне (сверху вниз): биты 3, 4, 5, 7
    if (bitGrid[startY + 0][startX + 0]) code |= (1 << 0);
    if (bitGrid[startY + 1][startX + 0]) code |= (1 << 1);
    if (bitGrid[startY + 2][startX + 0]) code |= (1 << 2);
    if (bitGrid[startY + 0][startX + 1]) code |= (1 << 3);
    if (bitGrid[startY + 1][startX + 1]) code |= (1 << 4);
    if (bitGrid[startY + 2][startX + 1]) code |= (1 << 5);
    if (bitGrid[startY + 3][startX + 0]) code |= (1 << 6);
    if (bitGrid[startY + 3][startX + 1]) code |= (1 << 7);

    int unicodeValue = 0x2800 + code;

    // Кодирование в UTF-8 string
    std::string utf8Str;
    utf8Str += static_cast<char>(0xE0 | ((unicodeValue >> 12) & 0x0F));
    utf8Str += static_cast<char>(0x80 | ((unicodeValue >> 6) & 0x3F));
    utf8Str += static_cast<char>(0x80 | (unicodeValue & 0x3F));
    return utf8Str;
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
        std::cerr << "Положите .obj файл модели рядом с бинарником!" << std::endl;
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

    // Выбираем 4 основных ракурса для одновременного вывода
    std::vector<ViewAngle> views = {
        {"FRONT",  0.0f,    0.0f},
        {"BACK",   0.0f,    3.14159f},
        {"LEFT",   0.0f,    4.71239f},
        {"RIGHT",  0.0f,    1.5708f}
    };

    // Общая текстовая консольная матрица
    std::vector<std::vector<std::string>> totalConsoleGrid(TOTAL_CHARS_H, std::vector<std::string>(TOTAL_CHARS_W, " "));

    // Отрисовка декоративных рамок-разделителей
    for (int x = 0; x < TOTAL_CHARS_W; ++x) {
        totalConsoleGrid[0][x] = "═";
        totalConsoleGrid[VIEW_CHARS_H + 1][x] = "═";
        totalConsoleGrid[TOTAL_CHARS_H - 1][x] = "═";
    }
    for (int y = 0; y < TOTAL_CHARS_H; ++y) {
        totalConsoleGrid[y][0] = "║";
        totalConsoleGrid[y][VIEW_CHARS_W + 1] = "║";
        totalConsoleGrid[y][TOTAL_CHARS_W - 1] = "║";
    }
    totalConsoleGrid[0][0] = "╔"; totalConsoleGrid[0][TOTAL_CHARS_W - 1] = "╗";
    totalConsoleGrid[TOTAL_CHARS_H - 1][0] = "╚"; totalConsoleGrid[TOTAL_CHARS_H - 1][TOTAL_CHARS_W - 1] = "╝";
    totalConsoleGrid[VIEW_CHARS_H + 1][0] = "╠"; totalConsoleGrid[VIEW_CHARS_H + 1][TOTAL_CHARS_W - 1] = "╣";
    totalConsoleGrid[0][VIEW_CHARS_W + 1] = "╦"; totalConsoleGrid[TOTAL_CHARS_H - 1][VIEW_CHARS_W + 1] = "╩";
    totalConsoleGrid[VIEW_CHARS_H + 1][VIEW_CHARS_W + 1] = "╬";

    // Обрабатываем каждый из 4-х ракурсов отдельно
    for (int vIdx = 0; vIdx < 4; ++vIdx) {
        const auto& view = views[vIdx];

        // Размеры битовой сетки высокого субпиксельного разрешения для одного ракурса
        int bitW = VIEW_CHARS_W * 2;
        int bitH = VIEW_CHARS_H * 4;

        std::vector<Vec3> transVertices(baseVertices.size());
        std::vector<std::pair<int, int>> subpixelCoords(baseVertices.size());

        for (size_t i = 0; i < baseVertices.size(); ++i) {
            Vec3 v = baseVertices[i];
            float x1 = v.x * cosf(view.rotY) + v.z * sinf(view.rotY);
            float z1 = -v.x * sinf(view.rotY) + v.z * cosf(view.rotY);
            float y2 = v.y * cosf(view.rotX) - z1 * sinf(view.rotX);
            float z2 = v.y * sinf(view.rotX) + z1 * cosf(view.rotX);

            transVertices[i] = {x1, y2, z2};

            // Проецируем вершины сразу в субпиксельную сетку ракурса
            subpixelCoords[i] = {
                static_cast<int>(x1 * SCALE_X + bitW / 2),
                static_cast<int>(-y2 * SCALE_Y + bitH / 2)
            };
        }

        for (auto& t : triangles) {
            Vec3 e1 = transVertices[t.vIdx[1]] - transVertices[t.vIdx[0]];
            Vec3 e2 = transVertices[t.vIdx[2]] - transVertices[t.vIdx[0]];
            t.faceNormal = e1.cross(e2).normalize();
            t.isFrontFacing = (t.faceNormal.z > 0.0f);
        }

        // Высокоточный Z-буфер субпиксельного уровня
        std::vector<float> zBuffer(bitW * bitH, -1e9f);
        for (const auto& t : triangles) {
            if (!t.isFrontFacing) continue;

            int x0 = subpixelCoords[t.vIdx[0]].first, y0 = subpixelCoords[t.vIdx[0]].second;
            int x1 = subpixelCoords[t.vIdx[1]].first, y1 = subpixelCoords[t.vIdx[1]].second;
            int x2 = subpixelCoords[t.vIdx[2]].first, y2 = subpixelCoords[t.vIdx[2]].second;

            int minX = std::max(0, std::min({x0, x1, x2}));
            int maxX = std::min(bitW - 1, std::max({x0, x1, x2}));
            int minY = std::max(0, std::min({y0, y1, y2}));
            int maxY = std::min(bitH - 1, std::max({y0, y1, y2}));

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
                        int idx = py * bitW + px;
                        if (z > zBuffer[idx]) zBuffer[idx] = z;
                    }
                }
            }
        }

        // Битовое поле субпикселей для текущего ракурса
        std::vector<std::vector<bool>> bitGrid(bitH, std::vector<bool>(bitW, false));
        const float CONTINUITY_THRESHOLD = 0.25f;

        for (const auto& e : edges) {
            bool isSilhouette = false;
            bool isCrease = false;

            bool f1 = (e.tri1 != -1) ? triangles[e.tri1].isFrontFacing : false;
            bool f2 = (e.tri2 != -1) ? triangles[e.tri2].isFrontFacing : false;

            if (e.tri2 == -1) isSilhouette = f1;
            else if (f1 != f2) isSilhouette = true;

            if (!isSilhouette && e.tri1 != -1 && e.tri2 != -1 && f1 && f2) {
                float dotNormal = triangles[e.tri1].faceNormal.dot(triangles[e.tri2].faceNormal);
                if (dotNormal < 0.20f) isCrease = true;
            }

            if (isSilhouette || isCrease) {
                int x0 = subpixelCoords[e.v1].first, y0 = subpixelCoords[e.v1].second;
                int x1 = subpixelCoords[e.v2].first, y1 = subpixelCoords[e.v2].second;
                float z0 = transVertices[e.v1].z;
                float z1 = transVertices[e.v2].z;

                const int steps = 16;
                int visiblePoints = 0;

                for (int s = 0; s <= steps; ++s) {
                    float t = (float)s / steps;
                    int currX = static_cast<int>(x0 + (x1 - x0) * t);
                    int currY = static_cast<int>(y0 + (y1 - y0) * t);
                    float currZ = z0 + (z1 - z0) * t;

                    if (currX >= 0 && currX < bitW && currY >= 0 && currY < bitH) {
                        int bufferIdx = currY * bitW + currX;
                        // Смягченный bias для субпиксельной сетки
                        if (zBuffer[bufferIdx] <= currZ + 0.015f) visiblePoints++;
                    }
                }

                float visibilityRate = static_cast<float>(visiblePoints) / (steps + 1);

                if (visibilityRate >= CONTINUITY_THRESHOLD) {
                    drawSubpixelLine(bitGrid, x0, y0, x1, y1, bitW, bitH);
                }
            }
        }

        // Определяем смещение ракурса на общем экране (Сетка 2х2)
        int charOffsetX = (vIdx % 2 == 0) ? 1 : VIEW_CHARS_W + 2;
        int charOffsetY = (vIdx / 2 == 0) ? 1 : VIEW_CHARS_H + 2;

        // Кодируем субпиксельные блоки 2х4 в символы Брайля и пишем в общую матрицу
        for (int cy = 0; cy < VIEW_CHARS_H; ++cy) {
            for (int cx = 0; cx < VIEW_CHARS_W; ++cx) {
                std::string brailleSymbol = encodeBrailleChar(bitGrid, cx * 2, cy * 4);

                // Если символ пустой (нет точек), оставляем пробел для читаемости
                if (brailleSymbol == "⠀") brailleSymbol = " ";

                totalConsoleGrid[charOffsetY + cy][charOffsetX + cx] = brailleSymbol;
            }
        }

        // Добавляем текстовую подпись ракурса в угол квадранта
        std::string label = "[" + view.name + "]";
        for (size_t l = 0; l < label.size(); ++l) {
            totalConsoleGrid[charOffsetY][charOffsetX + l + 1] = std::string(1, label[l]);
        }
    }

    // Сохраняем финальный консольный кадр со всеми 4 ракурсами в один .txt файл
    std::ofstream outFile("quad_console_lineart.txt");
    for (const auto& row : totalConsoleGrid) {
        for (const auto& symbol : row) {
            outFile << symbol;
        }
        outFile << "\n";
    }
    outFile.close();

    std::cout << "\n[ГОТОВО!] Сетка 2х2 ракурсов с субпиксельным качеством сохранена в 'quad_console_lineart.txt'" << std::endl;
    std::cout << "Открой файл в блокноте или выведи в терминал через 'cat'. Убедись, что шрифт поддерживает Unicode Braille (например, Consolas или Lucida Console)." << std::endl;

    return 0;
}
