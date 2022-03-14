#include "camera.hh"

#include <iostream>

Camera::Camera(Vector3 center, Vector3 focus, Vector3 up, double fov_v,
               double fov_h, double z_min)
{
    z_min_ = z_min;
    focus_ = focus;
    up_ = up;
    auto alpha = degrees_to_radians(fov_v);
    auto beta = degrees_to_radians(fov_h);

    viewport_v_ = 2.0 * tan(alpha) * z_min;
    viewport_h_ = 2.0 * tan(beta) * z_min;

    auto w = (focus - center).normalized();
    auto u = (cross(up, w)).normalized();
    auto v = cross(w, u);

    center_ = center;
    horizontal_ = viewport_h_ * u;
    vertical_ = viewport_v_ * v;
    lower_left_corner_ = center - horizontal_ / 2 - vertical_ / 2 + w;
    original_lower_left_corner_ = lower_left_corner_;
    original_vertical_ = vertical_;
    original_horizontal_ = horizontal_;
    original_focus_ = focus_ - center_;
    original_up_ = up_;
}

Ray Camera::get_ray(double x, double y) const
{
    return Ray(center_,
               (lower_left_corner_ + x * horizontal_ + y * vertical_)
                   - center_);
}

void Camera::set_position(Vector3 pos)
{
    Vector3 dist_to_screen = lower_left_corner_ - center_;
    center_ = pos;
    lower_left_corner_ = pos + dist_to_screen;
}

void Camera::set_rotation_y(double y)
{
    double beta = degrees_to_radians(y);

    double cx = original_lower_left_corner_.x() * cos(beta)
        + original_lower_left_corner_.z() * sin(beta);
    double cy = original_lower_left_corner_.y();
    double cz = -original_lower_left_corner_.x() * sin(beta)
        + original_lower_left_corner_.z() * cos(beta);

    lower_left_corner_ = Vector3(cx, cy, cz);

    double hx = original_horizontal_.x() * cos(beta)
        + original_horizontal_.z() * sin(beta);
    double hy = original_horizontal_.y();
    double hz = -original_horizontal_.x() * sin(beta)
        + original_horizontal_.z() * cos(beta);
    horizontal_ = Vector3(hx, hy, hz);
}

void Camera::set_rotation_x(double x)
{
    double alpha = degrees_to_radians(x);

    // rotation of x
    double cx = original_lower_left_corner_.x();
    double cy = original_lower_left_corner_.y() * cos(alpha)
        - original_lower_left_corner_.z() * sin(alpha);
    double cz = original_lower_left_corner_.y() * sin(alpha)
        + original_lower_left_corner_.z() * cos(alpha);
    lower_left_corner_ = Vector3(cx, cy, cz);

    double vx = original_vertical_.x();
    double vy = original_vertical_.y() * cos(alpha)
        - original_vertical_.z() * sin(alpha);
    double vz = original_vertical_.y() * sin(alpha)
        + original_vertical_.z() * cos(alpha);
    vertical_ = Vector3(vx, vy, vz);
}

void Camera::set_rotation_z(double z)
{
    double theta = degrees_to_radians(z);

    // rotation of z
    double zx = original_up_.x() * cos(theta) - original_up_.y() * sin(theta);
    double zy = original_up_.x() * sin(theta) + original_up_.y() * cos(theta);
    double zz = original_up_.z();
    up_ = Vector3(zx, zy, zz);
}
