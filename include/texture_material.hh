#pragma once

#include "color.hh"
#include "vector3.hh"

class Material
{
public:
    Material(Color color, float kd)
        : color_(color)
        , kd_(kd)
    {}

    Color get_color()
    {
        return color_;
    }

    float get_diffusion_coeff()
    {
        return kd_;
    }

private:
    Color color_;
    float kd_;
};

class Texture_Material
{
public:
    virtual Material get_Material(Vector3 point) = 0;

private:
};
