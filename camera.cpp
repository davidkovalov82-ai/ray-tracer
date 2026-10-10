#include "camera.h"

#include "ray.h"
#include "vec3.h"

Ray Camera::get_ray(double u, double v) {
    Vec3 hor, ver, cord, direction, ray;
    Ray result;

    hor = multiply(Camera::horizontal, u);
    ver = multiply(Camera::vertical, v);

    cord = add(Camera::lower_left_corner, hor);
    cord = add(cord, ver);

    direction = subtract(cord, Camera::origin);

    result.origin = origin;
    result.direction = direction;

    return result;
}