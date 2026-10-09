#include "vec3.h"

#include <cmath>
#include <stdexcept>

Vec3 add(const Vec3& a, const Vec3& b) {
    Vec3 result;

    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;
}

Vec3 subtract(const Vec3& a, const Vec3& b) {
    Vec3 result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}

Vec3 multiply(const Vec3& a, double num) {
    Vec3 result;

    result.x = a.x * num;
    result.y = a.y * num;
    result.z = a.z * num;

    return result;
}

Vec3 divide(const Vec3& a, double num) {
    Vec3 result;

    result.x = a.x / num;
    result.y = a.y / num;
    result.z = a.z / num;

    return result;
}

// Питагоровата теорема
double length(const Vec3& a) {
    double result;

    result = std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);

    return result;
}

// Нормализираме вектора: получаваме дължина 1.
Vec3 normalize(const Vec3& a) {
    const double len = length(a);

    if (len == 0.0) {
        throw std::invalid_argument("Нулев вектор не може да се нормализира");
    }

    Vec3 result;
    result.x = a.x / len;
    result.y = a.y / len;
    result.z = a.z / len;

    return result;
}
