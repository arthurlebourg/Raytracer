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

void Camera::set_rotation(Vector3 angle)
{
    double alpha = degrees_to_radians(angle.x());
    if (angle.x() > 90)
        alpha = degrees_to_radians(90);
    if (angle.x() < -90)
        alpha = degrees_to_radians(-90);
    double beta = degrees_to_radians(angle.y());
    double theta = degrees_to_radians(angle.z());
    // rotation of x
    double xx = original_focus_.x();
    double xy =
        original_focus_.y() * cos(alpha) - original_focus_.z() * sin(alpha);
    double xz =
        original_focus_.y() * sin(alpha) + original_focus_.z() * cos(alpha);
    Vector3 focus_rot_x = Vector3(xx, xy, xz);

    // rotation of y
    double yx =
        original_focus_.x() * cos(beta) + original_focus_.z() * sin(beta);
    double yy = original_focus_.y();
    double yz =
        -original_focus_.x() * sin(beta) + original_focus_.z() * cos(beta);
    focus_ = focus_rot_x + Vector3(yx, yy, yz) + center_;

    // rotation of z
    double zx = original_up_.x() * cos(theta) - original_up_.y() * sin(theta);
    double zy = original_up_.x() * sin(theta) + original_up_.y() * cos(theta);
    double zz = original_up_.z();
    up_ = Vector3(zx, zy, zz);

    auto w = (focus_ - center_).normalized();
    auto u = (cross(up_, w)).normalized();
    auto v = cross(w, u);

    horizontal_ = viewport_h_ * u;
    vertical_ = viewport_v_ * v;
    lower_left_corner_ = center_ - horizontal_ / 2 - vertical_ / 2 + w;
}
