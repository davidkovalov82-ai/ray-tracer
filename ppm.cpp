// P3
// 256 256
// 255
// 255 0 0
// 0 255 0
// ...

// P3 — означает, что это текстовый цветной формат.
// 256§ 256 — ширина и высота в пикселях.
// 255 — максимальное значение яркости (один байт на канал).

#include "ppm.h"

#include <fstream>
#include <string>
#include <vector>

#include "color.h"

bool save_ppm(const std::string& filename, int width, int height,
              const std::vector<Color>& pixels) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        return false;
    }

    out << "P3\n";
    out << width << " " << height << "\n";
    out << "255\n";
    for (const auto& pixel : pixels) {
        // внутри этого цикла переменная pixel по очереди
        // становится каждым пикселем из вектора pixels
        int ir = static_cast<int>(pixel.r * 255.0);
        int ig = static_cast<int>(pixel.g * 255.0);
        int ib = static_cast<int>(pixel.b * 255.0);
        out << ir << " " << ig << " " << ib << "\n";
    }

    return true;
}