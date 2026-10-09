#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <utility>
#include <vector>

#include "color.h"
#include "ppm.h"
#include "vec3.h"

int main(void) {
    const int image_width = 256;
    const int image_height = 256;

    std::vector<Color> pixels;
    pixels.reserve(image_height * image_width);

    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            // вычисляем цвет и добавляем в pixels
            double r = double(i) / (image_width - 1);
            double g = double(j) / (image_height - 1);
            double b = 0.0;
            pixels.push_back(Color{r, g, b});
        }
    }

    // Создаем директории, если их еще нет
    std::filesystem::create_directories("images/ppm");
    std::filesystem::create_directories("images/png");

    if (!save_ppm("images/ppm/image.ppm", image_width, image_height, pixels)) {
        std::cerr << "Error: could not save image.ppm\n";
        return 1;
    }
    std::cout << "PPM saved successfully to images/ppm/image.ppm\n";

    // Автоматическая конвертация в PNG утилитой macOS (sips)
    int status = std::system(
        "sips -s format png images/ppm/image.ppm --out "
        "images/png/image.png > /dev/null 2>&1");
    if (status == 0) {
        std::cout << "PNG saved successfully to images/png/image.png\n";
    } else {
        std::cerr << "Warning: could not convert PPM to PNG\n";
    }

    return 0;
}
