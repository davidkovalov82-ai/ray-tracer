#pragma once

#include "vec3.h"

struct Ray {
    Vec3 origin;
    Vec3 direction;
};

Vec3 ray_at(const Ray& ray, double t);