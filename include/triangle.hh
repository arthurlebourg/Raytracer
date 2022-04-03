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

    Triangle(const Triangle &t)
        : Object(t.texture_)
        , A_(t.A_)
        , B_(t.B_)
        , C_(t.C_)
    {}

    std::optional<Vector3> hit(Ray ray);

    Vector3 normal(Vector3 point);

    void move(Vector3 vec)
    {
        A_ = A_ + vec;
        B_ = B_ + vec;
        C_ = C_ + vec;
    }

    void set_position(Vector3 vec)
    {
        Vector3 AB = B_ - A_;
        Vector3 AC = C_ - A_;
        A_ = vec;
        B_ = vec + AB;
        C_ = vec + AC;
    }

    std::shared_ptr<Object> clone()
    {
        return std::make_shared<Triangle>(Triangle(*this));
    }

    bool is_transparent()
    {
        return false;
    }

    Vector3 get_center()
    {
        return Vector3((A_.x() + B_.x() + C_.x()) / 3,
                       (A_.y() + B_.y() + C_.y()) / 3,
                       (A_.z() + B_.z() + C_.z()) / 3);
    }

    void rescale(double scale)
    {
        A_ = A_ * scale;
        B_ = B_ * scale;
        C_ = C_ * scale;
    }

protected:
    Vector3 A_;
    Vector3 B_;
    Vector3 C_;
};
