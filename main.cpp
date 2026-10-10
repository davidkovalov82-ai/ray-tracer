#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <utility>
#include <vector>

#include "camera.h"
#include "color.h"
#include "ppm.h"
#include "ray.h"
#include "vec3.h"

Color ray_color(const Ray& ray);

Color ray_color(const Ray& ray) {
    Vec3 unit_direction = normalize(ray.direction);
    double t = (unit_direction.y + 1.0) / 2.0;

    Color white{1.0, 1.0, 1.0};
    Color blue{0.5, 0.7, 1.0};

    Color color;
    color.r = (1.0 - t) * white.r + t * blue.r;
    color.g = (1.0 - t) * white.g + t * blue.g;
    color.b = (1.0 - t) * white.b + t * blue.b;

    return color;
}

int main(void) {
    const int image_width = 256;
    const int image_height = 256;

    std::vector<Color> pixels;
    pixels.reserve(image_height * image_width);

    Camera camera;
    Ray ray;
    Color color;
    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            // вычисляем цвет и добавляем в pixels
            double u = double(i) / (image_width - 1);
            double v = 1 - double(j) / (image_height - 1);
            ray = camera.get_ray(u, v);
            color = ray_color(ray);
            pixels.push_back(color);
        }
    }

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
