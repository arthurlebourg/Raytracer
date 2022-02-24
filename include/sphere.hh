#pragma once

#include "object.hh"
#include "ray.hh"
#include "texture_material.hh"
#include "vector3.hh"

class Sphere : public Object
{
public:
    Sphere(Vector3 pos, float radius, Texture_Material texture)
        : Object(texture)
        , pos_(pos)
        , radius_(radius)
    {}

    std::optional<Vector3> hit(Ray ray);

    Vector3 normal(Vector3 point);

private:
    Vector3 pos_;
    float radius_;
};
