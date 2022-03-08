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

    Plane(const Plane &p)
        : Object(p.texture_)
        , pos_(p.pos_)
        , normal_(p.normal_)
    {}

    std::optional<Vector3> hit(Ray ray);

    Vector3 normal(Vector3 point);

    void move(Vector3 vec)
    {
        pos_ = pos_ + vec;
    }

    void set_position(Vector3 vec)
    {
        pos_ = vec;
    }

    std::shared_ptr<Object> clone()
    {
        return std::make_shared<Plane>(Plane(*this));
    }

private:
    Vector3 pos_;
    Vector3 normal_;
};
