#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../camera.h"
#include "../ray.h"

namespace {
constexpr double tolerance = 1e-10;
int checks = 0;

// Проверки выполняются и при сборке Release, в отличие от assert.
void expect_near(double actual, double expected, const std::string& name) {
    ++checks;
    if (!std::isfinite(actual) || std::abs(actual - expected) > tolerance) {
        throw std::runtime_error(name + ": expected " + std::to_string(expected) + ", got " +
                                 std::to_string(actual));
    }
}

void expect_vec(const Vec3& actual, const Vec3& expected, const std::string& name) {
    expect_near(actual.x, expected.x, name + ".x");
    expect_near(actual.y, expected.y, name + ".y");
    expect_near(actual.z, expected.z, name + ".z");
}
}  // namespace

int main() {
    try {
        Camera camera;
        struct Case {
            double u;
            double v;
            Vec3 direction;
            const char* name;
        };
        // Ожидаемые значения для текущего viewport 2 x 2 на z = -1.
        const Case cases[] = {
            {0.5, 0.5, {0.0, 0.0, -1.0}, "center"},
            {0.0, 0.0, {-1.0, -1.0, -1.0}, "bottom left"},
            {1.0, 0.0, {1.0, -1.0, -1.0}, "bottom right"},
            {0.0, 1.0, {-1.0, 1.0, -1.0}, "top left"},
            {1.0, 1.0, {1.0, 1.0, -1.0}, "top right"},
            {0.25, 0.75, {-0.5, 0.5, -1.0}, "fractional coordinates"},
            {0.5, 1.0, {0.0, 1.0, -1.0}, "top center"},
            {1.0, 0.5, {1.0, 0.0, -1.0}, "right center"},
        };
        for (const auto& item : cases) {
            const Ray ray = camera.get_ray(item.u, item.v);
            expect_vec(ray.origin, {0.0, 0.0, 0.0}, std::string(item.name) + " origin");
            expect_vec(ray.direction, item.direction, std::string(item.name) + " direction");
            expect_vec(ray_at(ray, 1.0), item.direction,
                       std::string(item.name) + " viewport point");
        }

        // Вызовы с другими координатами не должны изменять камеру.
        expect_vec(camera.get_ray(0.5, 0.5).direction, {0.0, 0.0, -1.0}, "center after other rays");
        std::cout << "PASS: " << checks << " camera checks\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}
