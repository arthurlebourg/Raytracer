#include "plane.hh"

std::optional<HitInfo> Plane::hit(Ray ray)
{
    Vector3 d = ray.direction();
    float denominator = dot(d, normal_);

    if (abs(denominator) < (2 ^ -52))
        return std::nullopt; // direction and plane parallel, no intersection

    float t = dot(pos_ - ray.origin(), normal_) / denominator;
    if (t < 0)
        return std::nullopt; // plane behind ray's origin

    return HitInfo(ray.origin() + ray.direction() * t, ray.direction(), this);
}

Vector3 Plane::normal(Vector3 point)
{
    point = point;
    return normal_;
}
