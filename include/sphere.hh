#pragma once

#include "hit_info.hh"
#include "object.hh"
#include "ray.hh"
#include "texture_material.hh"
#include "vector3.hh"

class Sphere : public Object
{
public:
    Sphere()
        : Object(nullptr)
        , pos_(Vector3())
        , radius_(0)
    {}

    Sphere(Vector3 pos, double radius,
           std::shared_ptr<Texture_Material> texture)
        : Object(texture)
        , pos_(pos)
        , radius_(radius)
    {}

    Sphere(const Sphere &s)
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
        return std::make_shared<Sphere>(Sphere(*this));
    }

    Vector3 get_center()
    {
        return pos_;
    }

    double get_radius()
    {
        return radius_;
    }

    void set_radius(double r)
    {
        radius_ = r;
    }

private:
    Vector3 pos_;
    double radius_;
};
