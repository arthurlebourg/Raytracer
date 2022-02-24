#include "sphere.hh"

Vector3 Sphere::hit(Vector3 point, Vector3 direction)
{
    Vector3 oc = point - pos_;
    float b = 2.0 * dot(oc, direction);
    float c = dot(oc, oc) - radius_ * radius_;
    float discriminant = b * b - 4 * c;
    if (discriminant < 0)
    {
        return nullptr;
    }
    else
    {
        float dist = (-b + sqrt(discriminant)) / 2.0;
        return point + direction * dist;
    }
}

Vector3 Sphere::normal(Vector3 point)
{
    return point - pos_;
}

Material Sphere::get_texture(Vector3 point)
{
    return texture_.get_Material(point);
}
