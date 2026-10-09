#pragma once

struct Vec3 {
    double x{0.0};
    double y{0.0};
    double z{0.0};
};

Vec3 add(const Vec3& a, const Vec3& b);
Vec3 subtract(const Vec3& a, const Vec3& b);
Vec3 multiply(const Vec3& a, double num);
Vec3 divide(const Vec3& a, double num);
double length(const Vec3& a);
Vec3 normalize(const Vec3& a);