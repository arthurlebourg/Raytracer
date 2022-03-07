#pragma once

#include "object.hh"
#include "ray.hh"
#include "texture_material.hh"
#include "vector3.hh"

class Triangle : public Object
{
public:
    Triangle(Vector3 A, Vector3 B, Vector3 C,
             std::shared_ptr<Texture_Material> texture)
        : Object(texture)
        , A_(A)
        , B_(B)
        , C_(C)
    {}

    std::optional<Vector3> hit(Ray ray);

    Vector3 normal(Vector3 point);

    void move(Vector3 vec)
    {
        A_ = A_ + vec;
        B_ = B_ + vec;
        C_ = C_ + vec;
    }

    void rescale(float scale)
    {
        A_ = A_ * scale;
        B_ = B_ * scale;
        C_ = C_ * scale;
    }

private:
    Vector3 A_;
    Vector3 B_;
    Vector3 C_;
};
