#include "sphere.hh"

std::optional<Vector3> Sphere::hit(Ray ray)
{
    Vector3 oc = ray.origin() - pos_;
    float b = 2.0 * dot(oc, ray.direction());
    float c = dot(oc, oc) - radius_ * radius_;
    float discriminant = b * b - 4 * c;
    if (discriminant < 0)
    {
        return std::nullopt;
    }
    else
    {
        float dist = (-b + sqrt(discriminant)) / 2.0;
        return ray.origin() + ray.direction() * dist;
    }
}

Vector3 Sphere::normal(Vector3 point)
{
    return point - pos_;
}
