#pragma once

#include <iostream>

#include "texture_material.hh"

class Uniform_Texture : public Texture_Material
{
public:
    Uniform_Texture(Material mat)
        : mat_(mat)
    {}

    Material get_Material(Vector3 point)
    {
        point = point;
        return mat_;
    }

private:
    Material mat_;
};
