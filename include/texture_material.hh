#pragma once

#include "color.hh"
#include "vector3.hh"

class Material
{
public:
    Material(Color color, float kd, float ks)
        : color_(color)
        , kd_(kd)
        , ks_(ks)
    {}

    Color get_color()
    {
        return color_;
    }

    float get_diffusion_coeff()
    {
        return kd_;
    }

    float ks()
    {
        return ks_;
    }

private:
    Color color_;
    float kd_;
    float ks_;
};

class Texture_Material
{
public:
    virtual Material get_Material(Vector3 point) = 0;

private:
};
