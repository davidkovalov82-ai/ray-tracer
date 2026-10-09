#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../vec3.h"

namespace {
constexpr double tolerance = 1e-10;
int checks = 0;

// Не използваме assert: проверките работят и в Release сборка.
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
        const Vec3 a{1.0, 2.0, 3.0};
        const Vec3 b{4.0, 5.0, 6.0};
        const Vec3 zero{0.0, 0.0, 0.0};

        expect_vec(add(a, b), {5.0, 7.0, 9.0}, "add");
        expect_vec(add(a, zero), a, "add zero");
        expect_vec(add({-1.5, 2.0, -3.0}, {0.5, -2.0, 4.0}), {-1.0, 0.0, 1.0}, "add signed values");

        expect_vec(subtract(a, b), {-3.0, -3.0, -3.0}, "subtract");
        expect_vec(subtract(b, a), {3.0, 3.0, 3.0}, "subtract reversed");
        expect_vec(subtract(a, a), zero, "subtract self");

        expect_vec(multiply(a, 2.0), {2.0, 4.0, 6.0}, "multiply");
        expect_vec(multiply(a, 0.5), {0.5, 1.0, 1.5}, "multiply fraction");
        expect_vec(multiply(a, -2.0), {-2.0, -4.0, -6.0}, "multiply negative");
        expect_vec(multiply(a, 0.0), zero, "multiply zero");

        // Текущият договор на divide изисква ненулев делител.
        expect_vec(divide({2.0, 4.0, 6.0}, 2.0), a, "divide");
        expect_vec(divide(a, 0.5), {2.0, 4.0, 6.0}, "divide fraction");
        expect_vec(divide({2.0, 4.0, 6.0}, -2.0), {-1.0, -2.0, -3.0}, "divide negative");
        expect_vec(divide(zero, 2.0), zero, "divide zero vector");

        expect_near(length({3.0, 4.0, 0.0}), 5.0, "length 3-4-5");
        expect_near(length({2.0, 3.0, 6.0}), 7.0, "length all components");
        expect_near(length({-3.0, -4.0, 0.0}), 5.0, "length negative");
        expect_near(length(zero), 0.0, "length zero");
        expect_near(length({0.3, 0.4, 0.0}), 0.5, "length fraction");

        const Vec3 unit = normalize({3.0, 4.0, 0.0});
        expect_vec(unit, {0.6, 0.8, 0.0}, "normalize");
        expect_near(length(unit), 1.0, "normalized length");
        expect_vec(normalize({-3.0, -4.0, 0.0}), {-0.6, -0.8, 0.0}, "normalize negative");
        expect_vec(normalize({0.0, 0.0, 7.0}), {0.0, 0.0, 1.0}, "normalize z axis");
        expect_vec(normalize(unit), unit, "normalize unit vector");

        bool rejected_zero = false;
        try {
            (void)normalize(zero);
        } catch (const std::invalid_argument&) {
            rejected_zero = true;
        }
        ++checks;
        if (!rejected_zero) {
            throw std::runtime_error("normalize zero: expected std::invalid_argument");
        }

        // Операциите връщат нов вектор и запазват входните данни.
        Vec3 input{1.0, 2.0, 3.0};
        Vec3 other{4.0, 5.0, 6.0};
        (void)add(input, other);
        (void)subtract(input, other);
        (void)multiply(input, 2.0);
        (void)divide(input, 2.0);
        (void)length(input);
        (void)normalize(input);
        expect_vec(input, a, "input unchanged");
        expect_vec(other, b, "second input unchanged");

        std::cout << "PASS: " << checks << " checks\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}