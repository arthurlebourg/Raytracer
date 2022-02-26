#pragma once

#include "ray.hh"
#include "utils.hh"
#include "vector3.hh"

class Camera
{
public:
    Camera(Vector3 center, Vector3 focus, Vector3 up, double fov_v,
           double fov_h, double z_min);

    Ray get_ray(double x, double y) const;

    Vector3 get_horizontal()
    {
        return horizontal_;
    }

    Vector3 get_vertical()
    {
        return vertical_;
    }

    Vector3 get_center()
    {
        return center_;
    }

    void change_pos(Vector3 pos)
    {
        center_ = center_ + pos;
        lower_left_corner_ = lower_left_corner_ + pos;
    }

private:
    Vector3 center_;
    Vector3 lower_left_corner_;
    Vector3 horizontal_;
    Vector3 vertical_;
};
