#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <thread>

// Масштабы "виртуального экрана" (текстуры, куда рендерится геометрия)
const int TEX_WIDTH = 160;
const int TEX_HEIGHT = 80;

// Масштабы консольного вывода (текстовые символы)
const int ASCII_WIDTH = 40;
const int ASCII_HEIGHT = 20;

// Размер одного ASCII-символа в пикселях текстуры
const int CELL_X = TEX_WIDTH / ASCII_WIDTH;   // 4 пикселя
const int CELL_Y = TEX_HEIGHT / ASCII_HEIGHT; // 4 пикселя

const float PI = 3.1415926535f;

struct Vec3 {
    float x, y, z;
    Vec3 operator+(const Vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    Vec3 operator-(const Vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
    Vec3 operator*(float f) const { return {x * f, y * f, z * f}; }
    Vec3 normalize() const {
        float len = std::sqrt(x*x + y*y + z*z);
        return len > 0 ? Vec3{x/len, y/len, z/len} : Vec3{0,0,0};
    }
};

// Ротация геометрии
Vec3 rotate(Vec3 p, float ax, float ay) {
    float s = std::sin(ax), c = std::cos(ax);
    float y1 = p.y * c - p.z * s, z1 = p.y * s + p.z * c;
    p.y = y1; p.z = z1;
    s = std::sin(ay); c = std::cos(ay);
    float x2 = p.x * c + p.z * s, z2 = -p.x * s + p.z * c;
    p.x = x2; p.z = z2;
    return p;
}

// Математическое описание тел (SDF) для Pass 1
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
    Vec3 rp = rotate(p, time * 0.4f, time * 0.7f);
    if (shape == 0) return sdCube(rp, {1.2f, 1.2f, 1.2f});
    if (shape == 1) return sdSphere(rp, 1.5f);
    return sdTorus(rp, 1.3f, 0.5f);
}

// --- PASS 1: Рендеринг сцены в текстурный буфер глубины/нормалей ---
void renderPass1(std::vector<float>& renderTex, int shape, float time) {
    for (int y = 0; y < TEX_HEIGHT; ++y) {
        float uv_y = (y - TEX_HEIGHT / 2.0f) / (TEX_HEIGHT / 2.0f);
        for (int x = 0; x < TEX_WIDTH; ++x) {
            float uv_x = (x - TEX_WIDTH / 2.0f) / (TEX_WIDTH / 2.0f) * (TEX_WIDTH / (float)TEX_HEIGHT) * 0.5f;

            Vec3 ro = {0, 0, -5};
            Vec3 rd = Vec3{uv_x, uv_y, 1.2f}.normalize();

            float t = 0;
            bool hit = false;
            for (int i = 0; i < 35; ++i) {
                Vec3 p = ro + rd * t;
                float d = map(p, shape, time);
                if (d < 0.002f) { hit = true; break; }
                t += d;
                if (t > 8.0f) break;
            }

            // Вместо цвета записываем псевдоглубину (резкие перепады дадут идеальный контур)
            renderTex[y * TEX_WIDTH + x] = hit ? (10.0f - t) : 0.0f;
        }
    }
}

// Выбор символа по градиенту (аналог логики выбора символа)
char getAsciiComponent(float sx, float sy, float threshold) {
    float g = sx * sx + sy * sy;
    if (g < threshold) return ' ';

    float angle = std::atan2(sy, sx);
    float degrees = angle * 180.0f / PI;
    if (degrees < 0) degrees += 180.0f;

    if (degrees >= 67.5f && degrees < 112.5f)  return '-';
    if (degrees >= 112.5f && degrees < 157.5f) return '/';
    if (degrees >= 22.5f && degrees < 67.5f)   return '\\';
    return '|';
}

// --- PASS 2: Пост-процессинг Собелем + Даунсэмплинг в ASCII ---
void renderPass2(const std::vector<float>& renderTex, float edgeThreshold) {
    const int Kx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    const int Ky[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    std::string asciiFrame = "";

    // Шагаем по ASCII-сетке (ячейками CELL_X на CELL_Y)
    for (int ay = 0; ay < ASCII_HEIGHT; ++ay) {
        for (int ax = 0; ax < ASCII_WIDTH; ++ax) {

            // Берем центральный пиксель внутри текущей ASCII-ячейки в текстуре
            int texX = ax * CELL_X + CELL_X / 2;
            int texY = ay * CELL_Y + CELL_Y / 2;

            // Защита от выхода за границы текстуры при свертке 3х3
            if (texX < 1 || texX >= TEX_WIDTH - 1 || texY < 1 || texY >= TEX_HEIGHT - 1) {
                asciiFrame += ' ';
                continue;
            }

            // Ядро Собеля (как texelFetchOffset в твоем фрагментном шейдере)
            float sx = 0, sy = 0;
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    float val = renderTex[(texY + ky) * TEX_WIDTH + (texX + kx)];
                    sx += val * Kx[ky + 1][kx + 1];
                    sy += val * Ky[ky + 1][kx + 1];
                }
            }

            // Получаем финальный символ лайн-арта для этой ячейки
            asciiFrame += getAsciiComponent(sx, sy, edgeThreshold);
        }
        asciiFrame += '\n';
    }

    // Рендеринг кадра в консоль (ANSI код возврата курсора в начало)
    std::cout << "\x1b[H" << asciiFrame;
}

int main() {
    // Очистка экрана и скрытие текстового курсора
    std::cout << "\x1b[2J\x1b[?25l";

    // Наш CPU аналог RenderTex (текстурный буфер)
    std::vector<float> renderTex(TEX_WIDTH * TEX_HEIGHT, 0.0f);

    float time = 0.0f;
    float edgeThreshold = 1.2f; // Порог чувствительности граней (подбирается под сцену)

    while (true) {
        // Каждые 6 секунд меняем фигуру: 0 - Куб, 1 - Сфера, 2 - Тор
        int currentShape = (static_cast<int>(time) / 6) % 3;

        // Pass 1: Запись геометрии в буфер текстуры
        renderPass1(renderTex, currentShape, time);

        // Pass 2: Применение фильтра Собеля к текстуре и вывод ASCII
        renderPass2(renderTex, edgeThreshold);

        time += 0.07f;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
    return 0;
}
