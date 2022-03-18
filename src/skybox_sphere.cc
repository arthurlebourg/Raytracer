#include "skybox_sphere.hh"

std::optional<Vector3> Skybox_Sphere::hit(Ray ray)
{
    Vector3 oc = pos_ - ray.origin();
    double b = dot(oc, ray.direction());
    if (b < 0) // check if obj is behind cam
    {
        return std::nullopt;
    }
    double a = dot(ray.direction(), ray.direction());
    double c = oc.squaredNorm() - (radius_ * radius_);
    double discriminant = b * b - a * c;
    if (discriminant < 0)
    {
        // std::cout << "delta: " << discriminant << std::endl;
        return std::nullopt;
    }
    else
    {
        double dist = (-b + sqrt(discriminant)) / a;
        // return ray.origin() + ray.direction() * -dist;
        return ray.origin() + ray.direction() * -dist;
    }
}

Vector3 Skybox_Sphere::normal(Vector3 point)
{
    return (pos_ - point).normalized();
}
