#pragma once

#include "raytracer/ray.hh"
#include "utils/utils.hh"

class Camera
{
public:
    Camera(Vector3 center, Vector3 focus, Vector3 up, double fov_v,
           double fov_h, double z_min);

    Ray get_ray(double x, double y) const;

    Vector3 get_horizontal() const
    {
        return horizontal_;
    }

    Vector3 get_vertical() const
    {
        return vertical_;
    }

    Vector3 get_center() const
    {
        return center_;
    }

    Vector3 get_up() const
    {
        return up_;
    }

    Vector3 get_focus() const
    {
        return focus_;
    }

    void change_pos(Vector3 pos)
    {
        center_ = center_ + pos;
        lower_left_corner_ = lower_left_corner_ + pos;
    }

    void set_position(Vector3 pos);

    void set_rotation_x(double x);
    void set_rotation_y(double y);
    void set_rotation_z(double z);

private:
    Vector3 center_;
    Vector3 lower_left_corner_;
    Vector3 horizontal_;
    Vector3 vertical_;
    Vector3 focus_;
    Vector3 up_;

    Vector3 original_lower_left_corner_;
    Vector3 original_horizontal_;
    Vector3 original_vertical_;
    Vector3 original_focus_;
    Vector3 original_up_;

    double z_min_;
    double viewport_v_;
    double viewport_h_;
};
