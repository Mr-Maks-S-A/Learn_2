// #include <iostream>
// #include <vector>
// #include <cmath>
// #include <algorithm>
// #include <chrono>
// #include <thread>
//
// // Масштабы "виртуального экрана" (текстуры высокого разрешения)
// const int TEX_WIDTH = 160;
// const int TEX_HEIGHT = 80;
//
// // Масштабы консольного вывода (низкое разрешение для ASCII)
// const int ASCII_WIDTH = 40;
// const int ASCII_HEIGHT = 20;
//
// // Размер ячейки сэмплинга (сколько пикселей текстуры сжимаются в один символ)
// const int CELL_X = TEX_WIDTH / ASCII_WIDTH;   // 4 пикселя
// const int CELL_Y = TEX_HEIGHT / ASCII_HEIGHT; // 4 пикселя
//
// const float PI = 3.1415926535f;
//
// // Простая структура для работы с 3D векторами
// struct Vec3 {
//     float x, y, z;
//     Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
//     Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
//     Vec3 operator*(float f) const { return {x * f, y * f, z * f}; }
//     float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
//     Vec3 normalize() const {
//         float len = std::sqrt(x*x + y*y + z*z);
//         return len > 0 ? Vec3{x/len, y/len, z/len} : Vec3{0,0,0};
//     }
// };
//
// // Функция вращения трехмерной точки в пространстве
// Vec3 rotate(Vec3 p, float ax, float ay) {
//     float s = std::sin(ax), c = std::cos(ax);
//     float y1 = p.y * c - p.z * s, z1 = p.y * s + p.z * c;
//     p.y = y1; p.z = z1;
//     s = std::sin(ay); c = std::cos(ay);
//     float x2 = p.x * c + p.z * s, z2 = -p.x * s + p.z * c;
//     p.x = x2; p.z = z2;
//     return p;
// }
//
// // --- МАТЕМАТИЧЕСКОЕ ОПИСАНИЕ ФИГУР (SDF) ---
// float sdCube(Vec3 p, Vec3 b) {
//     Vec3 q = {std::abs(p.x) - b.x, std::abs(p.y) - b.y, std::abs(p.z) - b.z};
//     return std::sqrt(std::max(q.x,0.0f)*std::max(q.x,0.0f) + std::max(q.y,0.0f)*std::max(q.y,0.0f) + std::max(q.z,0.0f)*std::max(q.z,0.0f)) + std::min(std::max(q.x,std::max(q.y,q.z)),0.0f);
// }
// float sdSphere(Vec3 p, float r) { return std::sqrt(p.x*p.x + p.y*p.y + p.z*p.z) - r; }
// float sdTorus(Vec3 p, float r1, float r2) {
//     float qx = std::sqrt(p.x*p.x + p.z*p.z) - r1;
//     return std::sqrt(qx*qx + p.y*p.y) - r2;
// }
//
// // Глобальная карта сцены
// float map(Vec3 p, int shape, float time) {
//     Vec3 rp = rotate(p, time * 0.4f, time * 0.7f);
//     if (shape == 0) return sdCube(rp, {1.1f, 1.1f, 1.1f});
//     if (shape == 1) return sdSphere(rp, 1.4f);
//     return sdTorus(rp, 1.2f, 0.5f);
// }
//
// // Вычисление точных нормалей (необходимо для прорисовки внутренних ребер)
// Vec3 getNormal(Vec3 p, int shape, float time) {
//     float eps = 0.001f;
//     float d = map(p, shape, time);
//     float nx = map({p.x + eps, p.y, p.z}, shape, time) - d;
//     float ny = map({p.x, p.y + eps, p.z}, shape, time) - d;
//     float nz = map({p.x, p.y, p.z + eps}, shape, time) - d;
//     return Vec3{nx, ny, nz}.normalize();
// }
//
// // --- PASS 1: Рендеринг геометрии в буфер текстуры (Комбинация глубины и нормалей) ---
// void renderPass1(std::vector<float>& renderTex, int shape, float time) {
//     for (int y = 0; y < TEX_HEIGHT; ++y) {
//         float uv_y = (y - TEX_HEIGHT / 2.0f) / (TEX_HEIGHT / 2.0f);
//         for (int x = 0; x < TEX_WIDTH; ++x) {
//             // Корректируем aspect ratio, чтобы применить масштабирование
//             float uv_x = (x - TEX_WIDTH / 2.0f) / (TEX_WIDTH / 2.0f) * (TEX_WIDTH / (float)TEX_HEIGHT) * 0.5f;
//
//             Vec3 ro = {0, 0, -5}; // Позиция камеры
//             Vec3 rd = Vec3{uv_x, uv_y, 1.3f}.normalize(); // Луч
//
//             float t = 0;
//             bool hit = false;
//             Vec3 p;
//
//             for (int i = 0; i < 40; ++i) {
//                 p = ro + rd * t;
//                 float d = map(p, shape, time);
//                 if (d < 0.001f) { hit = true; break; }
//                 t += d;
//                 if (t > 8.0f) break;
//             }
//
//             if (hit) {
//                 Vec3 n = getNormal(p, shape, time);
//                 float depthVal = (10.0f - t); // Базовая глубина
//
//                 // Магическая формула: подмешиваем нормали к глубине.
//                 // На стыках граней куба или складках тора значения резко изменятся, и Собель их увидит!
//                 renderTex[y * TEX_WIDTH + x] = depthVal * 0.4f + (n.x * 1.5f) + (n.y * 1.5f) + (n.z * 0.5f);
//             } else {
//                 renderTex[y * TEX_WIDTH + x] = 0.0f; // Пустое пространство
//             }
//         }
//     }
// }
//
// // Выбор конкретного ASCII-символа на основе градиента и средней глубины
// char getAsciiComponent(float sx, float sy, float threshold, float avgDepth) {
//     float g = sx * sx + sy * sy;
//
//     // 1. Отрисовка линий (границы/ребра геометрии)
//     if (g >= threshold) {
//         float angle = std::atan2(sy, sx);
//         float degrees = angle * 180.0f / PI;
//         if (degrees < 0) degrees += 180.0f;
//
//         if (degrees >= 67.5f && degrees < 112.5f)  return '-';
//         if (degrees >= 112.5f && degrees < 157.5f) return '/';
//         if (degrees >= 22.5f && degrees < 67.5f)   return '\\';
//         return '|';
//     }
//
//     // 2. Мягкое тоновое затенение (Dithering) для улучшения понимания объема внутри контуров
//     if (avgDepth > 0.1f) {
//         if (avgDepth > 2.8f) return '.'; // Слегка заполняем ближние плоскости точками
//     }
//
//     return ' ';
// }
//
// // --- PASS 2: Суперсэмплинг (SSAA), Фильтр Собеля и вывод на экран ---
// void renderPass2(const std::vector<float>& renderTex, float edgeThreshold) {
//     const int Kx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
//     const int Ky[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
//
//     std::string asciiFrame = "";
//
//     // Сканируем сетку ASCII символов
//     for (int ay = 0; ay < ASCII_HEIGHT; ++ay) {
//         for (int ax = 0; ax < ASCII_WIDTH; ++ax) {
//
//             float avg_sx = 0, avg_sy = 0;
//             float totalDepth = 0;
//             int samples = 0;
//
//             // СУПЕРСЭМПЛИНГ: Просчитываем Собеля для ВСЕХ пикселей внутри ячейки 4x4
//             for (int cy = 0; cy < CELL_Y; ++cy) {
//                 for (int cx = 0; cx < CELL_X; ++cx) {
//                     int texX = ax * CELL_X + cx;
//                     int texY = ay * CELL_Y + cy;
//
//                     // Игнорируем края текстуры, чтобы избежать выхода за массив
//                     if (texX < 1 || texX >= TEX_WIDTH - 1 || texY < 1 || texY >= TEX_HEIGHT - 1) continue;
//
//                     float sx = 0, sy = 0;
//                     // Свертка ядра Собеля (Аналог texelFetchOffset из GLSL)
//                     for (int ky = -1; ky <= 1; ++ky) {
//                         for (int kx = -1; kx <= 1; ++kx) {
//                             float val = renderTex[(texY + ky) * TEX_WIDTH + (texX + kx)];
//                             sx += val * Kx[ky + 1][kx + 1];
//                             sy += val * Ky[ky + 1][kx + 1];
//                         }
//                     }
//                     avg_sx += sx;
//                     avg_sy += sy;
//                     totalDepth += renderTex[texY * TEX_WIDTH + texX];
//                     samples++;
//                 }
//             }
//
//             if (samples > 0) {
//                 avg_sx /= samples;
//                 avg_sy /= samples;
//                 totalDepth /= samples;
//             }
//
//             // Получаем итоговый символ для ячейки консоли
//             asciiFrame += getAsciiComponent(avg_sx, avg_sy, edgeThreshold, totalDepth);
//         }
//         asciiFrame += '\n';
//     }
//
//     // Мгновенный вывод кадра (ANSI код \x1b[H возвращает курсор в верхний левый угол без мерцания)
//     std::cout << "\x1b[H" << asciiFrame;
// }
//
// int main() {
//     // Подготовка консоли: очистка экрана и скрытие мигающего курсора
//     std::cout << "\x1b[2J\x1b[?25l";
//
//     // Наш двумерный буфер-текстура (Pass 1 Destination)
//     std::vector<float> renderTex(TEX_WIDTH * TEX_HEIGHT, 0.0f);
//
//     float time = 0.0f;
//     // Оптимальный порог чувствительности Собеля для масштаба 40x20
//     float edgeThreshold = 0.45f;
//
//     while (true) {
//         // Каждые 5 секунд автоматически переключаем фигуру: 0 - Куб, 1 - Сфера, 2 - Тор
//         int currentShape = (static_cast<int>(time) / 5) % 3;
//
//         // 1 Проход: Считаем физику 3D и пишем нормали+глубину в текстуру высокого разрешения
//         renderPass1(renderTex, currentShape, time);
//
//         // 2 Проход: Прогоняем Собелем, усредняем SSAA-сэмплом и выводим Line Art символы
//         renderPass2(renderTex, edgeThreshold);
//
//         time += 0.06f;
//         std::this_thread::sleep_for(std::chrono::milliseconds(33)); // ~30 FPS
//     }
//
//     return 0;
// }















#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <thread>
#include <string>

// Размеры виртуальной текстуры (High-Res для точного Собеля)
const int TEX_WIDTH = 160;
const int TEX_HEIGHT = 80;

// Размеры вывода в консоль (Low-Res для ASCII)
const int ASCII_WIDTH = 40;
const int ASCII_HEIGHT = 20;

const int CELL_X = TEX_WIDTH / ASCII_WIDTH;   // 4 пикселя
const int CELL_Y = TEX_HEIGHT / ASCII_HEIGHT; // 4 пикселя

const float PI = 3.1415926535f;

// Битовые маски "портов подключения" для проверки замкнутости
const int PORT_NONE  = 0;
const int PORT_UP    = 1 << 0; // 1
const int PORT_DOWN  = 1 << 1; // 2
const int PORT_LEFT  = 1 << 2; // 4
const int PORT_RIGHT = 1 << 3; // 8

// Структура Юникод-символа с метаданными о его геометрии
struct UnicodeStroke {
    std::string symbol;
    int ports; // Какие стороны этот символ связывает
};

// Простая структура 3D-вектора
struct Vec3 {
    float x, y, z;
    Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vec3 operator*(float f) const { return {x * f, y * f, z * f}; }
    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 normalize() const {
        float len = std::sqrt(x*x + y*y + z*z);
        return len > 0 ? Vec3{x/len, y/len, z/len} : Vec3{0,0,0};
    }
};

// Матрица вращения
Vec3 rotate(Vec3 p, float ax, float ay) {
    float s = std::sin(ax), c = std::cos(ax);
    float y1 = p.y * c - p.z * s, z1 = p.y * s + p.z * c;
    p.y = y1; p.z = z1;
    s = std::sin(ay); c = std::cos(ay);
    float x2 = p.x * c + p.z * s, z2 = -p.x * s + p.z * c;
    p.x = x2; p.z = z2;
    return p;
}

// --- ГЕОМЕТРИЯ (SDF) ---
float sdCube(Vec3 p, Vec3 b) {
    Vec3 q = {std::abs(p.x) - b.x, std::abs(p.y) - b.y, std::abs(p.z) - b.z};
    return std::sqrt(std::max(q.x,0.0f)*std::max(q.x,0.0f) + std::max(q.y,0.0f)*std::max(q.y,0.0f) + std::max(q.z,0.0f)*std::max(q.z,0.0f)) + std::min(std::max(q.x,std::max(q.y,q.z)),0.0f);
}
float sdSphere(Vec3 p, float r) { return std::sqrt(p.x*p.x + p.y*p.y + p.z*p.z) - r; }
float sdTorus(Vec3 p, float r1, float r2) {
    float qx = std::sqrt(p.x*p.x + p.z*p.z) - r1;
    return std::sqrt(qx*qx + p.y*p.y) - r2;
}

float map(Vec3 p, int shape, float time) {
    Vec3 rp = rotate(p, time * 0.4f, time * 0.6f);
    if (shape == 0) return sdCube(rp, {1.1f, 1.1f, 1.1f});
    if (shape == 1) return sdSphere(rp, 1.4f);
    return sdTorus(rp, 1.2f, 0.5f);
}

Vec3 getNormal(Vec3 p, int shape, float time) {
    float eps = 0.001f;
    float d = map(p, shape, time);
    float nx = map({p.x + eps, p.y, p.z}, shape, time) - d;
    float ny = map({p.x, p.y + eps, p.z}, shape, time) - d;
    float nz = map({p.x, p.y, p.z + eps}, shape, time) - d;
    return Vec3{nx, ny, nz}.normalize();
}

// --- PASS 1: Рендеринг в буфер (Глубина + Изломы Нормалей) ---
void renderPass1(std::vector<float>& renderTex, int shape, float time) {
    for (int y = 0; y < TEX_HEIGHT; ++y) {
        float uv_y = (y - TEX_HEIGHT / 2.0f) / (TEX_HEIGHT / 2.0f);
        for (int x = 0; x < TEX_WIDTH; ++x) {
            float uv_x = (x - TEX_WIDTH / 2.0f) / (TEX_WIDTH / 2.0f) * (TEX_WIDTH / (float)TEX_HEIGHT) * 0.5f;

            Vec3 ro = {0, 0, -5};
            Vec3 rd = Vec3{uv_x, uv_y, 1.3f}.normalize();

            float t = 0;
            bool hit = false;
            Vec3 p;

            for (int i = 0; i < 40; ++i) {
                p = ro + rd * t;
                float d = map(p, shape, time);
                if (d < 0.001f) { hit = true; break; }
                t += d;
                if (t > 8.0f) break;
            }

            if (hit) {
                Vec3 n = getNormal(p, shape, time);
                float depthVal = (10.0f - t);
                // Кодируем пространственные перепады формы
                renderTex[y * TEX_WIDTH + x] = depthVal * 0.4f + (n.x * 1.5f) + (n.y * 1.5f) + (n.z * 0.5f);
            } else {
                renderTex[y * TEX_WIDTH + x] = 0.0f;
            }
        }
    }
}

// --- PASS 2: Собель, Анализ связей соседних ячеек и Юникод-подстановка ---
void renderPass2(const std::vector<float>& renderTex, float edgeThreshold) {
    const int Kx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    const int Ky[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Карты первичных данных ASCII-экрана
    std::vector<int> initialDirections(ASCII_WIDTH * ASCII_HEIGHT, 0);
    std::vector<float> depthMap(ASCII_WIDTH * ASCII_HEIGHT, 0.0f);

    // Этап А: Вычисление градиентов методом SSAA суперсэмплинга
    for (int ay = 0; ay < ASCII_HEIGHT; ++ay) {
        for (int ax = 0; ax < ASCII_WIDTH; ++ax) {
            float avg_sx = 0, avg_sy = 0;
            float totalDepth = 0;
            int samples = 0;

            for (int cy = 0; cy < CELL_Y; ++cy) {
                for (int cx = 0; cx < CELL_X; ++cx) {
                    int texX = ax * CELL_X + cx;
                    int texY = ay * CELL_Y + cy;
                    if (texX < 1 || texX >= TEX_WIDTH - 1 || texY < 1 || texY >= TEX_HEIGHT - 1) continue;

                    float sx = 0, sy = 0;
                    for (int ky = -1; ky <= 1; ++ky) {
                        for (int kx = -1; kx <= 1; ++kx) {
                            float val = renderTex[(texY + ky) * TEX_WIDTH + (texX + kx)];
                            sx += val * Kx[ky + 1][kx + 1];
                            sy += val * Ky[ky + 1][kx + 1];
                        }
                    }
                    avg_sx += sx; avg_sy += sy;
                    totalDepth += renderTex[texY * TEX_WIDTH + texX];
                    samples++;
                }
            }

            if (samples > 0) {
                avg_sx /= samples; avg_sy /= samples; totalDepth /= samples;
            }

            float g = avg_sx * avg_sx + avg_sy * avg_sy;
            int idx = ay * ASCII_WIDTH + ax;
            depthMap[idx] = totalDepth;

            if (g >= edgeThreshold) {
                float angle = std::atan2(avg_sy, avg_sx);
                float degrees = angle * 180.0f / PI;
                if (degrees < 0) degrees += 180.0f;

                // Базовая классификация направлений (1: Горизонталь, 2: Вертикаль, 3: Наклон /, 4: Наклон \)
                if (degrees >= 67.5f && degrees < 112.5f)       initialDirections[idx] = 1;
                else if (degrees >= 112.5f && degrees < 157.5f) initialDirections[idx] = 3;
                else if (degrees >= 22.5f && degrees < 67.5f)   initialDirections[idx] = 4;
                else                                            initialDirections[idx] = 2;
            }
        }
    }

    // Этап Б: Анализ топологии и сборка геометрии
    std::string asciiFrame = "";

    for (int ay = 0; ay < ASCII_HEIGHT; ++ay) {
        for (int ax = 0; ax < ASCII_WIDTH; ++ax) {
            int idx = ay * ASCII_WIDTH + ax;

            int current = initialDirections[idx];

            // Считываем типы линий соседей (безопасный доступ с защитой краев)
            int left  = (ax > 0) ? initialDirections[idx - 1] : 0;
            int right = (ax < ASCII_WIDTH - 1) ? initialDirections[idx + 1] : 0;
            int up    = (ay > 0) ? initialDirections[idx - ASCII_WIDTH] : 0;
            int down  = (ay < ASCII_HEIGHT - 1) ? initialDirections[idx + ASCII_WIDTH] : 0;

            // 1. Обработка ПУСТЫХ зон внутри объектов (Плитки объема)
            if (current == 0) {
                if (depthMap[idx] > 3.2f) {
                    // Разреженный минималистичный паттерн внутренних ядер для читаемости
                    if (ax % 4 == 0 && ay % 2 == 0) { asciiFrame += "❍"; continue; }
                }
                asciiFrame += " ";
                continue;
            }

            // 2. АЛГОРИТМ УМНОЙ СТЫКОВКИ (Проверка замкнутости линий)
            // Если детекция Собеля выдала прямую линию, но вокруг нее есть перпендикулярные соседи,
            // мы динамически "загибаем" ее в Юникод-уголок.

            if (current == 1 || current == 2) { // Если базово определился прямой отрезок
                if ((down == 2 || down == 4) && (right == 1 || right == 4)) { asciiFrame += "╭"; continue; }
                if ((down == 2 || down == 3) && (left == 1 || left == 3))   { asciiFrame += "╮"; continue; }
                if ((up == 2 || up == 3) && (right == 1 || right == 3))     { asciiFrame += "╰"; continue; }
                if ((up == 2 || up == 4) && (left == 1 || left == 4))       { asciiFrame += "╯"; continue; }
            }

            // 3. Если это чистые изолированные наклоны — выводим длинные векторные слэши
            if (current == 3) { asciiFrame += "╱"; continue; }
            if (current == 4) { asciiFrame += "╲"; continue; }

            // 4. Обычные прямые линии (если они не превратились в углы)
            if (current == 1) asciiFrame += "─";
            else if (current == 2) asciiFrame += "│";
            else asciiFrame += " ";
        }
        asciiFrame += '\n';
    }

    // Рендеринг кадра в консоль (без мерцания за счет возврата каретки)
    std::cout << "\x1b[H" << asciiFrame;
}

int main() {
    // Активация UTF-8 для корректного отображения векторных Юникод-символов в Windows-терминале
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif

    // Очистка экрана консоли и гашение текстового курсора
    std::cout << "\x1b[2J\x1b[?25l";

    std::vector<float> renderTex(TEX_WIDTH * TEX_HEIGHT, 0.0f);
    float time = 0.0f;
    float edgeThreshold = 0.48f; // Баланс чувствительности лайн-арта

    while (true) {
        // Циклическая смена фигур каждые 5 секунд: Куб -> Сфера -> Тор
        int currentShape = (static_cast<int>(time) / 5) % 3;

        // Первый проход: генерация геометрии в растр высокого разрешения
        renderPass1(renderTex, currentShape, time);

        // Второй проход: фильтрация, анализ портов связности и сборка Unicode Line Art
        renderPass2(renderTex, edgeThreshold);

        time += 0.05f;
        std::this_thread::sleep_for(std::chrono::milliseconds(33)); // Фиксация ~30 FPS
    }

    return 0;
}
