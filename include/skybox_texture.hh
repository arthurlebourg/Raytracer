#pragma once

#include <iostream>

#include "texture_material.hh"

class Skybox_Texture : public Texture_Material
{
public:
    Skybox_Texture()
    {}

    Material get_Material(Vector3 point)
    {
        point = point;
        float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        Color default_color = r > 0.98 ? Color(255, 255, 255) : Color(0, 0, 0);
        return Material(default_color, 1, 1);
    }

private:
};
