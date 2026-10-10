#pragma once
#include "ray.h"
#include "vec3.h"

class Camera {
    Vec3 origin{0, 0, 0};
    Vec3 lower_left_corner{-1, -1, -1};
    Vec3 horizontal{2, 0, 0};
    Vec3 vertical{0, 2, 0};

public:
    Ray get_ray(double u, double v);
};
