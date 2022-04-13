#pragma once

#include <cmath>
#include <iostream>

#include "fastnoise.hh"
#include "image.hh"
#include "texture_material.hh"
#include "utils.hh"

class Alien_Texture : public Texture_Material
{
public:
    Alien_Texture(int seed, size_t res_x, size_t res_y)
        : seed_(seed)
        , res_x_(res_x)
        , res_y_(res_y)
    {
        noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        noise_.SetSeed(seed);
        tex = (Color *)calloc(sizeof(Color), res_x * res_y);
        Image texture = Image("tex_alien.ppm", res_x, res_y);
        for (size_t y = 0; y < res_y_; y++)
        {
            for (size_t x = 0; x < res_x_; x++)
            {
                double terrain =
                    sum_octave(noise_, 0, 16, x, y, 0.4, 0.3, 0, 255);
                Color c = Color(terrain, 0, 0);
                tex[y * res_x_ + x] = c;
                texture.set(c, x, y);
            }
        }
        texture.save();
    }

    Material get_Material(Vector3 point, Vector3 center,
                          double light_specular_intensity)
    {
        Vector3 normal = (center - point).normalized();

        size_t px = mercator(normal, res_x_, res_y_);

        if (px > res_x_ * res_y_)
            std::cout << "texture out of bound earth" << std::endl;

        double r = tex[px].red();
        Color grass(196, 8, 209);
        Color sand(219, 13, 2);
        Color snow(237, 218, 9);
        Color water(31, 206, 237);
        Color c = grass;
        if (r <= 125)
            c = sand;
        if (r <= 120)
            c = water;
        if (r >= 200)
            c = snow;
        return Material(c * light_specular_intensity, 1, 1);
    }

private:
    int seed_;
    FastNoiseLite noise_;
    const size_t res_x_;
    const size_t res_y_;
    Color *tex;
};
