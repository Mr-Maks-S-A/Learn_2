#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <fstream>

// Подключаем GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Реализация библиотеки stb_image_write для PNG
// В одном из файлов проекта перед инклюдом обязательно должен быть этот дефайн:
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct Vertex {
    glm::vec3 position;
};

struct Triangle {
    int v0, v1, v2;
    glm::vec3 normal;
};

struct Edge {
    int v0, v1;
    int tri1 = -1;
    int tri2 = -1;
};

class LineArtRenderer {
public:
    int width, height;
    std::vector<std::string> screen_buffer;

    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
    std::vector<Edge> edges;

    LineArtRenderer(int w, int h) : width(w), height(h) {
        clear_buffer();
    }

    void clear_buffer() {
        screen_buffer.assign(height, std::string(width, ' '));
    }

    void add_edge(int v0, int v1, int tri_idx) {
        if (v0 > v1) std::swap(v0, v1);
        for (auto& edge : edges) {
            if (edge.v0 == v0 && edge.v1 == v1) {
                edge.tri2 = tri_idx;
                return;
            }
        }
        edges.push_back({v0, v1, tri_idx, -1});
    }

    void generate_cube() {
        vertices = {
            {{-1, -1, -1}}, {{ 1, -1, -1}}, {{ 1,  1, -1}}, {{-1,  1, -1}},
            {{-1, -1,  1}}, {{ 1, -1,  1}}, {{ 1,  1,  1}}, {{-1,  1,  1}}
        };
        int indices[12][3] = {
            {0,1,2}, {0,2,3}, {4,5,6}, {4,6,7},
            {0,4,7}, {0,7,3}, {1,5,6}, {1,6,2},
            {3,2,6}, {3,6,7}, {0,1,5}, {0,5,4}
        };

        for (int i = 0; i < 12; ++i) {
            Triangle tri{indices[i][0], indices[i][1], indices[i][2], glm::vec3(0)};
            glm::vec3 edge1 = vertices[tri.v1].position - vertices[tri.v0].position;
            glm::vec3 edge2 = vertices[tri.v2].position - vertices[tri.v0].position;
            tri.normal = glm::normalize(glm::cross(edge1, edge2));

            triangles.push_back(tri);
            int tri_idx = triangles.size() - 1;

            add_edge(tri.v0, tri.v1, tri_idx);
            add_edge(tri.v1, tri.v2, tri_idx);
            add_edge(tri.v2, tri.v0, tri_idx);
        }
    }

    void draw_line_ascii(int x0, int y0, int x1, int y1) {
        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        char edge_char = '#';
        if (dx > 2 * dy) edge_char = '-';
        else if (dy > 2 * dx) edge_char = '|';
        else if ((sx == 1 && sy == 1) || (sx == -1 && sy == -1)) edge_char = '\\';
        else edge_char = '/';

        while (true) {
            if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height) {
                screen_buffer[y0][x0] = edge_char;
            }
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x0 += sx; }
            if (e2 < dx)  { err += dx; y0 += sy; }
        }
    }

    // Сохранение текущего ASCII буфера в ч/б PNG-изображение
    void save_to_png(const std::string& filename) {
        // Создаем одномерный массив байт для пикселей (1 канал - Grayscale)
        std::vector<uint8_t> pixel_data(width * height);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Если символ не пробел, считаем его линией (белый пиксель), иначе — фон (черный)
                if (screen_buffer[y][x] != ' ') {
                    pixel_data[y * width + x] = 255; // Белый
                } else {
                    pixel_data[y * width + x] = 0;   // Черный
                }
            }
        }

        // Записываем файл: имя, ширина, высота, количество каналов (1), указатель на данные, шаг (ширина)
        int success = stbi_write_png(filename.c_str(), width, height, 1, pixel_data.data(), width);

        if (success) {
            // Опционально: выводим лог в консоль ниже кадра
            std::cout << "Frame saved to " << filename << std::endl;
        }
    }

    void render(float angle_y, float angle_x) {
        clear_buffer();

        glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle_y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, angle_x, glm::vec3(1.0f, 0.0f, 0.0f));

        // Для PNG лучше использовать квадратные пропорции пикселя,
        // поэтому убрали разницу масштабирования X и Y (сделали симметрично)
        auto project = [&](glm::vec3 p) {
            glm::vec4 transformed = model * glm::vec4(p, 1.0f);
            int x = static_cast<int>((transformed.x * 1.5f + 1.0f) * 0.5f * (width - 1));
            int y = static_cast<int>((transformed.y * 1.5f + 1.0f) * 0.5f * (height - 1));
            return glm::ivec2(x, height - 1 - y);
        };

        glm::vec3 view_dir = glm::vec3(0.0f, 0.0f, 1.0f);

        std::vector<glm::vec3> trans_normals(triangles.size());
        std::vector<bool> tri_visible(triangles.size(), false);

        for (size_t i = 0; i < triangles.size(); ++i) {
            glm::vec3 n = glm::mat3(model) * triangles[i].normal;
            trans_normals[i] = n;
            tri_visible[i] = glm::dot(n, view_dir) > 0.0f;
        }

        for (const auto& edge : edges) {
            bool is_line = false;

            if (edge.tri2 == -1) {
                if (tri_visible[edge.tri1]) is_line = true;
            } else {
                bool v1 = tri_visible[edge.tri1];
                bool v2 = tri_visible[edge.tri2];

                if (v1 != v2) {
                    is_line = true;
                }
                else if (v1 && v2) {
                    float dot_normals = glm::dot(trans_normals[edge.tri1], trans_normals[edge.tri2]);
                    if (dot_normals < std::cos(glm::radians(45.0f))) {
                        is_line = true;
                    }
                }
            }

            if (is_line) {
                glm::ivec2 p0 = project(vertices[edge.v0].position);
                glm::ivec2 p1 = project(vertices[edge.v1].position);
                draw_line_ascii(p0.x, p0.y, p1.x, p1.y);
            }
        }

        std::string out = "";
        for (const auto& row : screen_buffer) {
            out += row + "\n";
        }
        std::cout << "\x1b[H" << out;
    }
};

int main() {
    // Для PNG-картинки сделаем разрешение побольше (например, 512x512)
    int img_size = 512;
    LineArtRenderer renderer(img_size, img_size);
    renderer.generate_cube();

    float angle_y = 0.0f;
    float angle_x = 0.4f;

    std::cout << "\x1b[2J";

    for (int frame = 0; frame < 100; ++frame) {
        renderer.render(angle_y, angle_x);

        // Каждые 10 кадров сохраняем снимок в PNG
        if (frame % 10 == 0) {
            renderer.save_to_png("line_art_frame_" + std::to_string(frame) + ".png");
        }

        angle_y += 0.05f;
        angle_x += 0.02f;

        struct timespec ts = {0, 30000000};
        nanosleep(&ts, NULL);
    }

    return 0;
}
