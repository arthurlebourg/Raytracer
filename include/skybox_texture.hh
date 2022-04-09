#pragma once

#include <cmath>
#include <iostream>

#include "image.hh"
#include "noise.hh"
#include "texture_material.hh"
#include "utils.hh"

class Skybox_Texture : public Texture_Material
{
public:
    Skybox_Texture(int res_x, int res_y, Color *texture_map)
        : seed_(0)
        , res_x_(res_x)
        , res_y_(res_y)
        , tex(texture_map)
    {}

    Skybox_Texture(int seed, int res_x, int res_y);

    Material get_Material(Vector3 point, Vector3 center,
                          double light_specular_intensity);

    Color *get_texture()
    {
        return tex;
    }

private:
    int seed_;
    FastNoiseLite noise_;
    const size_t res_x_;
    const size_t res_y_;
    Color *tex;
};
