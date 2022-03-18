#include "sphere.hh"

std::optional<Vector3> Sphere::hit(Ray ray)
{
    // if ray is inside sphere
    double sq_dst_to_center = (ray.origin() - pos_).squaredNorm();
    int val = 1;
    if (sq_dst_to_center < radius_ * radius_)
    {
        val = -1;
    }

    Vector3 oc = pos_ - ray.origin();
    double b = dot(oc, ray.direction());
    /*if (b < 0) // check if obj is behind cam
    {
        return std::nullopt;
    }*/
    double a = dot(ray.direction(), ray.direction());
    double c = oc.squaredNorm() - (radius_ * radius_);
    double discriminant = b * b - a * c;
    if (discriminant < 0)
    {
        return std::nullopt;
    }
    else
    {
        double dist = (-b + val * sqrt(discriminant)) / a;
        return ray.origin() + ray.direction() * -dist;
    }
}

Vector3 Sphere::normal(Vector3 point)
{
    return (point - pos_).normalized();
}
