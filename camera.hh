#pragma once

class Camera
{
public:
    Camera(Vector3 center, Vector3 front, Vector3 up, float fov_alpha,
           float fov_beta, float z_min)
        : center_(center)
        , front_(front)
        , up_(up)
        , fov_alpha_(fov_alpha)
        , fov_beta_(fov_beta)
        , z_min_(z_min)
    {}

    Ray get_ray(int x, int y);

private:
    Vector3 center_;
    Vector3 front_;
    Vector3 up_;
    float fov_alpha_;
    float fov_beta_;
    float z_min_;

    Vector3 lower_left_corner_;
    Vector3 horizontal_;
    Vector3 vertical_;
};
