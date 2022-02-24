#pragma once

#include <iostream>

#include "texture_material.hh"

class Uniform_Texture : public Texture_Material
{
public:
    Uniform_Texture(Material mat)
        : mat_(mat)
    {}

    Material get_Texture(Vector3 point)
    {
        return mat_;
    }

private:
    Material mat_;
};
