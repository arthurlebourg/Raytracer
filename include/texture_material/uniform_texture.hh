#pragma once

#include <iostream>

#include "texture_material/texture_material.hh"

class Uniform_Texture : public Texture_Material
{
public:
    Uniform_Texture(Material mat)
        : mat_(mat)
    {}

    Material get_Material(Vector3 point, Vector3 center,
                          double light_specular_intensity)
    {
        center = center;
        point = point;
        return Material(mat_.get_color() * light_specular_intensity,
                        mat_.get_diffusion_coeff(), mat_.ks());
    }

private:
    Material mat_;
};
