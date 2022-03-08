#include "camera.hh"

#include <iostream>

Camera::Camera(Vector3 center, Vector3 focus, Vector3 up, double fov_v,
               double fov_h, double z_min)
{
    auto alpha = degrees_to_radians(fov_v);
    auto beta = degrees_to_radians(fov_h);

    auto viewport_v = 2.0 * tan(alpha) * z_min;
    auto viewport_h = 2.0 * tan(beta) * z_min;

    auto w = (focus - center).normalized();
    auto u = (cross(up, w)).normalized();
    auto v = cross(w, u);

    center_ = center;
    horizontal_ = viewport_h * u;
    vertical_ = viewport_v * v;
    lower_left_corner_ = center - horizontal_ / 2 - vertical_ / 2 + w;
}

Ray Camera::get_ray(double x, double y) const
{
    return Ray(center_,
               (lower_left_corner_ + x * horizontal_ + y * vertical_)
                   - center_);
}
