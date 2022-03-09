#pragma once

#include "hit_info.hh"
#include "object.hh"
#include "ray.hh"
#include "texture_material.hh"
#include "vector3.hh"

class Skybox_Sphere : public Object
{
public:
    Skybox_Sphere(Vector3 pos, float radius,
                  std::shared_ptr<Texture_Material> texture)
        : Object(texture)
        , pos_(pos)
        , radius_(radius)
    {}

    Skybox_Sphere(const Skybox_Sphere &s)
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
        return std::make_shared<Skybox_Sphere>(Skybox_Sphere(*this));
    }

    Vector3 get_center()
    {
        return pos_;
    }

private:
    Vector3 pos_;
    float radius_;
};
