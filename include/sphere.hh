#pragma once

#include "hit_info.hh"
#include "object.hh"
#include "ray.hh"
#include "texture_material.hh"
#include "vector3.hh"

class Sphere : public Object
{
public:
    Sphere(Vector3 pos, float radius, std::shared_ptr<Texture_Material> texture)
        : Object(texture)
        , pos_(pos)
        , radius_(radius)
    {}

    std::optional<Vector3> hit(Ray ray);

    Vector3 normal(Vector3 point);

    void move(Vector3 vec)
    {
        pos_ = pos_ + vec;
    }

private:
    Vector3 pos_;
    float radius_;
};
