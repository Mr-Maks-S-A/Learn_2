#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

// Размеры ОДНОГО ракурса в символах под интерфейс (30 строк, 15 колонок)
const int VIEW_CHARS_W = 100;
const int VIEW_CHARS_H = 100;

// Размеры всей сетки 2х2 в символах (включая рамки)
const int TOTAL_CHARS_W = VIEW_CHARS_W * 2 + 3;
const int TOTAL_CHARS_H = VIEW_CHARS_H * 2 + 3;

// Масштаб для Stone Story ASCII (камера отдалена, чтобы модель гарантированно влезала)
const float SCALE_X = VIEW_CHARS_W * 0.12f;
// В ASCII-арте высота строки обычно в 2 раза больше ширины символа, компенсируем это:
const float SCALE_Y = VIEW_CHARS_H * 0.12f * 0.5f;

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

// Выбор ASCII символа в зависимости от направления линии (как в Stone Story)
char getAsciiCharForLine(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0; // В консоли Y идет сверху вниз

    if (dx == 0 && dy == 0) return '.';

    float angle = std::atan2(static_cast<float>(dy), static_cast<float>(dx)) * 180.0f / 3.14159265f;
    if (angle < 0) angle += 180.0f; // Приводим к диапазону 0-180

    // Разделяем углы на сектора для подбора символа
    if (angle >= 0.0f && angle < 22.5f)   return '_';
    if (angle >= 22.5f && angle < 67.5f)  return '\\'; // Наклон влево-вниз
    if (angle >= 67.5f && angle < 112.5f) return '|';  // Вертикаль
    if (angle >= 112.5f && angle < 157.5f) return '/';  // Наклон вправо-вниз
    return '_';
}

// Рисование линии Брезенхэма с заполнением ASCII текстурой
void drawAsciiLine(std::vector<std::vector<char>>& grid, std::vector<std::vector<float>>& zGrid,
                   int x0, int y0, int x1, int y1, float z0, float z1) {
    char edgeChar = getAsciiCharForLine(x0, y0, x1, y1);

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    int steps = std::max(dx, dy);
    int stepCount = 0;

    while (true) {
        if (x0 >= 0 && x0 < VIEW_CHARS_W && y0 >= 0 && y0 < VIEW_CHARS_H) {
            float t = (steps == 0) ? 1.0f : static_cast<float>(stepCount) / steps;
            float currentZ = z0 + (z1 - z0) * t;

            // Z-буфер для символов (учитываем небольшой bias для пересекающихся ребер)
            if (currentZ > zGrid[y0][x0] - 0.02f) {
                zGrid[y0][x0] = currentZ;
                grid[y0][x0] = edgeChar;
            }
        }
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx)  { err += dx; y0 += sy; }
        stepCount++;
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
                       std::vector<Triangle> triangles = loadOBJ("moto_simple_1.obj", baseVertices);
                       if (triangles.empty()) {
                           std::cerr << "Положите suzanne.obj рядом с исполняемым файлом!" << std::endl;
                           return 1;
                       }

                       // Авто-центрирование модели
                       Vec3 minV = {1e9f, 1e9f, 1e9f}, maxV = {-1e9f, -1e9f, -1e9f};
                       for (const auto& v : baseVertices) {
                           minV.x = std::min(minV.x, v.x); minV.y = std::min(minV.y, v.y); minV.z = std::min(minV.z, v.z);
                           maxV.x = std::max(maxV.x, v.x); maxV.y = std::max(maxV.y, v.y); maxV.z = std::max(maxV.z, v.z);
                       }
                       Vec3 center = (minV + maxV) * 0.5f;
                       for (auto& v : baseVertices) v = v - center;

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

                       // Отрисовка ретро-рамок интерфейса
                       for (int x = 0; x < TOTAL_CHARS_W; ++x) {
                           totalConsoleGrid[0][x] = "-";
                           totalConsoleGrid[VIEW_CHARS_H + 1][x] = "-";
                           totalConsoleGrid[TOTAL_CHARS_H - 1][x] = "-";
                       }
                       for (int y = 0; y < TOTAL_CHARS_H; ++y) {
                           totalConsoleGrid[y][0] = "|";
                           totalConsoleGrid[y][VIEW_CHARS_W + 1] = "|";
                           totalConsoleGrid[y][TOTAL_CHARS_W - 1] = "|";
                       }
                       totalConsoleGrid[0][0] = "+"; totalConsoleGrid[0][TOTAL_CHARS_W - 1] = "+";
                       totalConsoleGrid[TOTAL_CHARS_H - 1][0] = "+"; totalConsoleGrid[TOTAL_CHARS_H - 1][TOTAL_CHARS_W - 1] = "+";
                       totalConsoleGrid[VIEW_CHARS_H + 1][0] = "+"; totalConsoleGrid[VIEW_CHARS_H + 1][TOTAL_CHARS_W - 1] = "+";
                       totalConsoleGrid[0][VIEW_CHARS_W + 1] = "+"; totalConsoleGrid[TOTAL_CHARS_H - 1][VIEW_CHARS_W + 1] = "+";
                       totalConsoleGrid[VIEW_CHARS_H + 1][VIEW_CHARS_W + 1] = "+";

                       // Текстовые метки ракурсов врезанные в рамки
                       totalConsoleGrid[0][2] = "F"; totalConsoleGrid[0][3] = "R";
                       totalConsoleGrid[0][VIEW_CHARS_W + 3] = "B"; totalConsoleGrid[0][VIEW_CHARS_W + 4] = "A";
                       totalConsoleGrid[VIEW_CHARS_H + 1][2] = "L"; totalConsoleGrid[VIEW_CHARS_H + 1][3] = "E";
                       totalConsoleGrid[VIEW_CHARS_H + 1][VIEW_CHARS_W + 3] = "R"; totalConsoleGrid[VIEW_CHARS_H + 1][VIEW_CHARS_W + 4] = "I";

                       for (int vIdx = 0; vIdx < 4; ++vIdx) {
                           const auto& view = views[vIdx];

                           std::vector<Vec3> transVertices(baseVertices.size());
                           std::vector<std::pair<int, int>> charCoords(baseVertices.size());

                           for (size_t i = 0; i < baseVertices.size(); ++i) {
                               Vec3 v = baseVertices[i];
                               float x1 = v.x * cosf(view.rotY) + v.z * sinf(view.rotY);
                               float z1 = -v.x * sinf(view.rotY) + v.z * cosf(view.rotY);
                               float y2 = v.y * cosf(view.rotX) - z1 * sinf(view.rotX);
                               float z2 = v.y * sinf(view.rotX) + z1 * cosf(view.rotX);

                               transVertices[i] = {x1, y2, z2};

                               // Проекция напрямую в символьные координаты 15х30
                               charCoords[i] = {
                                   static_cast<int>(x1 * SCALE_X + VIEW_CHARS_W / 2),
                                   static_cast<int>(-y2 * SCALE_Y + VIEW_CHARS_H / 2)
                               };
                           }

                           for (auto& t : triangles) {
                               Vec3 e1 = transVertices[t.vIdx[1]] - transVertices[t.vIdx[0]];
                               Vec3 e2 = transVertices[t.vIdx[2]] - transVertices[t.vIdx[0]];
                               t.faceNormal = e1.cross(e2).normalize();
                               t.isFrontFacing = (t.faceNormal.z > 0.0f);
                           }

                           // Локальные буферы кадра для текущего ракурса
                           std::vector<std::vector<char>> localViewGrid(VIEW_CHARS_H, std::vector<char>(VIEW_CHARS_W, ' '));
                           std::vector<std::vector<float>> localZBuffer(VIEW_CHARS_H, std::vector<float>(VIEW_CHARS_W, -1e9f));

                           for (const auto& e : edges) {
                               bool isSilhouette = false;
                               bool isCrease = false;

                               bool f1 = (e.tri1 != -1) ? triangles[e.tri1].isFrontFacing : false;
                               bool f2 = (e.tri2 != -1) ? triangles[e.tri2].isFrontFacing : false;

                               if (e.tri2 == -1) isSilhouette = f1;
                               else if (f1 != f2) isSilhouette = true;

                               if (!isSilhouette && e.tri1 != -1 && e.tri2 != -1 && f1 && f2) {
                                   float dotNormal = triangles[e.tri1].faceNormal.dot(triangles[e.tri2].faceNormal);
                                   if (dotNormal < 0.3f) isCrease = true;
                               }

                               if (isSilhouette || isCrease) {
                                   int x0 = charCoords[e.v1].first, y0 = charCoords[e.v1].second;
                                   int x1 = charCoords[e.v2].first, y1 = charCoords[e.v2].second;
                                   float z0 = transVertices[e.v1].z;
                                   float z1 = transVertices[e.v2].z;

                                   drawAsciiLine(localViewGrid, localZBuffer, x0, y0, x1, y1, z0, z1);
                               }
                           }

                           // Перенос локального ракурса на глобальный экран интерфейса
                           int charOffsetX = (vIdx % 2 == 0) ? 1 : VIEW_CHARS_W + 2;
                           int charOffsetY = (vIdx / 2 == 0) ? 1 : VIEW_CHARS_H + 2;

                           for (int cy = 0; cy < VIEW_CHARS_H; ++cy) {
                               for (int cx = 0; cx < VIEW_CHARS_W; ++cx) {
                                   totalConsoleGrid[charOffsetY + cy][charOffsetX + cx] = std::string(1, localViewGrid[cy][cx]);
                               }
                           }
                       }

                       // Сохранение кадра
                       std::ofstream outFile("stone_story_style_ui.txt");
                       for (const auto& row : totalConsoleGrid) {
                           for (const auto& symbol : row) outFile << symbol;
                           outFile << "\n";
                       }
                       outFile.close();

                       std::cout << "[УСПЕХ] Чистый ASCII-вариант (как в Stone Story) сохранен в 'stone_story_style_ui.txt'!" << std::endl;
                       return 0;
}
