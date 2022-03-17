#pragma once

#include <iostream>
#include <memory>
#include <optional>

#include "ray.hh"
#include "texture_material.hh"

class Object
{
public:
    virtual std::optional<Vector3> hit(Ray ray) = 0;

    virtual Vector3 normal(Vector3 point) = 0;

    virtual void move(Vector3 vec) = 0;

    virtual void set_position(Vector3 vec) = 0;

    virtual std::shared_ptr<Object> clone() = 0;

    Material get_texture(Vector3 point)
    {
        return texture_->get_Material(point, get_center());
    }

    void set_texture(std::shared_ptr<Texture_Material> texture);
    {
        texture_ = texture;
    }

    virtual Vector3 get_center() = 0;

protected:
    std::shared_ptr<Texture_Material> texture_;
    Object(std::shared_ptr<Texture_Material> texture)
        : texture_(texture)
    {}
};
