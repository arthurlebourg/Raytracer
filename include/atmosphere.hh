#pragma once

#include "hit_info.hh"
#include "object.hh"
#include "ray.hh"
#include "texture_material.hh"
#include "vector3.hh"

class Atmosphere : public Object
{
public:
    Atmosphere(Vector3 pos, double radius,
               std::shared_ptr<Texture_Material> texture)
        : Object(texture)
        , pos_(pos)
        , radius_(radius)
    {}

    Atmosphere(const Atmosphere &s)
        : Object(s.texture_)
        , pos_(s.pos_)
        , radius_(s.radius_)
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
        return std::make_shared<Atmosphere>(Atmosphere(*this));
    }

    bool is_transparent()
    {
        return true;
    }

    Vector3 get_center()
    {
        return pos_;
    }

    double get_radius()
    {
        return radius_;
    }

private:
    Vector3 pos_;
    double radius_;
};
