#pragma once

#include <iostream>

#include "texture_material.hh"

class Sun_Texture : public Texture_Material
{
public:
    Sun_Texture(Material mat)
        : mat_(mat)
    {}

    Material get_Material(Vector3 point, Vector3 center,
                          double light_specular_intensity)
    {
        center = center;
        point = point;
        light_specular_intensity = light_specular_intensity;
        return Material(mat_.get_color(), mat_.get_diffusion_coeff(),
                        mat_.ks());
    }

private:
    Material mat_;
};
