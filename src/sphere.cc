#include "sphere.hh"

std::optional<Vector3> Sphere::hit(Ray ray)
{
    Vector3 oc = ray.origin() - pos_;
    float a = dot(ray.direction(), ray.direction());
    float b = 2.0 * dot(ray.direction(), oc);
    float c = dot(oc, oc) - (radius_ * radius_);
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        // std::cout << "delta: " << discriminant << std::endl;
        return std::nullopt;
    }
    else
    {
        float dist = (-b + sqrt(discriminant)) / (2.0 * a);
        // std::cout << "dist: " << dist << " -b: " << -b << " sqrt(delta): " <<
        // sqrt(discriminant) << std::endl;
        return ray.origin() + ray.direction() * dist;
    }
}

Vector3 Sphere::normal(Vector3 point)
{
    return point - pos_;
}
