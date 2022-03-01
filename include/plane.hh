#pragma once

#include "object.hh"
#include "ray.hh"
#include "texture_material.hh"
#include "vector3.hh"

class Plane : public Object
{
public:
    Plane(Vector3 pos, Vector3 normal,
          std::shared_ptr<Texture_Material> texture)
        : Object(texture)
        , pos_(pos)
        , normal_(normal)
    {}

    std::optional<HitInfo> hit(Ray ray);

    Vector3 normal(Vector3 point);

    void move(Vector3 vec)
    {
        pos_ = pos_ + vec;
    }

private:
    Vector3 pos_;
    Vector3 normal_;
};
