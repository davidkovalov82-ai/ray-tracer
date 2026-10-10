#include "ray.h"

#include "vec3.h"

Vec3 ray_at(const Ray& ray, double t) {
    Vec3 product, result;

    product = multiply(ray.direction, t);
    result = add(ray.origin, product);

    return result;
}