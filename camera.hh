#pragma once

#include "utils.hh"
#include "vector3.hh"

class Camera
{
public:
    Camera(Vector3 center, Vector3 focus, Vector3 up, float fov_v, float fov_h,
           float z_min);

    Ray get_ray(int x, int y) const;

private:
    Vector3 center_;
    Vector3 lower_left_corner_;
    Vector3 horizontal_;
    Vector3 vertical_;
};
