#include "triangle.hh"

#include <iostream>

std::optional<Vector3> Triangle::hit(Ray ray)
{
    const float EPSILON = 0.0000001;
    Vector3 edge1 = B_ - A_;
    Vector3 edge2 = C_ - A_;
    Vector3 normal = cross(ray.direction(), edge2);
    float a = dot(edge1, normal);
    if (a > -EPSILON && a < EPSILON)
    {
        return std::nullopt;
    }

    float f = 1.0 / a;
    Vector3 AO = ray.origin() - A_;
    float u = f * (dot(AO, normal));
    if (u < 0.0 || u > 1.0)
    {
        return std::nullopt;
    }

    Vector3 q = cross(AO, edge1);
    float v = f * dot(ray.direction(), q);
    if (v < 0.0 || u + v > 1.0)
    {
        return std::nullopt;
    }

    float dist = f * dot(edge2, q);
    if (dist > EPSILON)
    {
        return ray.origin() + ray.direction() * dist;
    }

    return std::nullopt;
}

Vector3 Triangle::normal(Vector3 point)
{
    point = point;
    Vector3 edge1 = A_ - B_;
    Vector3 edge2 = A_ - C_;
    return cross(edge1, edge2).normalized();
}
