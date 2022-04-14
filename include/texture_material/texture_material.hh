#pragma once

#include "raytracer/vector3.hh"
#include "texture_material/color.hh"

class Material
{
public:
    Material(Color color, double kd, double ks)
        : color_(color)
        , kd_(kd)
        , ks_(ks)
    {}

    Color get_color()
    {
        return color_;
    }

    double get_diffusion_coeff()
    {
        return kd_;
    }

    double ks()
    {
        return ks_;
    }

private:
    Color color_;
    double kd_;
    double ks_;
};

class Texture_Material
{
public:
    virtual Material get_Material(Vector3 point, Vector3 center,
                                  double light_specular_intensity) = 0;

private:
};
