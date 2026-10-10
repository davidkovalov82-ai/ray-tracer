#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../ray.h"

namespace {
constexpr double tolerance = 1e-10;

void expect_near(double actual, double expected, const std::string& name) {
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
        Ray ray;
        ray.origin = {1.0, 2.0, 3.0};
        ray.direction = {2.0, -1.0, 0.5};

        // t = 0: получаем начало луча.
        expect_vec(ray_at(ray, 0.0), {1.0, 2.0, 3.0}, "ray_at zero");

        // t = 1: прибавляем направление один раз.
        expect_vec(ray_at(ray, 1.0), {3.0, 1.0, 3.5}, "ray_at one");

        // Проверяем масштабирование направления.
        expect_vec(ray_at(ray, 2.0), {5.0, 0.0, 4.0}, "ray_at two");

        // Дробное значение t.
        expect_vec(ray_at(ray, 0.5), {2.0, 1.5, 3.25}, "ray_at fraction");

        // Отрицательное t: точка позади начала.
        expect_vec(ray_at(ray, -1.0), {-1.0, 3.0, 2.5}, "ray_at negative");

        // Проверяем, что исходный луч не изменился.
        expect_vec(ray.origin, {1.0, 2.0, 3.0}, "origin unchanged");
        expect_vec(ray.direction, {2.0, -1.0, 0.5}, "direction unchanged");

        // Направление длины 2: нормализовать его не требуется.
        Ray second_ray;
        second_ray.origin = {-2.0, 1.0, 4.0};
        second_ray.direction = {0.0, 0.0, -2.0};

        expect_vec(ray_at(second_ray, 3.0), {-2.0, 1.0, -2.0}, "ray_at non-unit direction");

        std::cout << "PASS: all ray tests passed\n";
        return 0;

    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}