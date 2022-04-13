#pragma once

#include <cmath>
#include <iostream>

#include "image.hh"
#include "noise.hh"
#include "texture_material.hh"
#include "utils.hh"

class Earth_Texture : public Texture_Material
{
public:
    Earth_Texture(int seed, size_t res_x, size_t res_y);

    Material get_Material(Vector3 point, Vector3 center,
                          double light_specular_intensity)
    {
        Vector3 normal = (center - point).normalized();

        size_t px = mercator(normal, res_x_, res_y_);

        if (px > res_x_ * res_y_)
            std::cout << "texture out of bound earth" << std::endl;

        double r = tex[px].red();
        Color grass(0, 154, 22);
        Color sand(248, 214, 114);
        Color snow(255, 250, 250);
        Color water(0, 84, 148);
        Color c = grass;
        if (r <= 125)
            c = sand;
        if (r <= 120)
            c = water;
        if (r >= 200)
            c = snow;
        if (light_specular_intensity <= 0.3 && tex[px].green() > 0)
        {
            Color city_lights(255, 255, 155);
            double coef = (tex[px].green() / 255.0);
            return Material(city_lights * coef, 1, 1);
        }
        return Material(c * light_specular_intensity, 1, 1);
    }

private:
    int seed_;
    FastNoiseLite noise_;
    const size_t res_x_;
    const size_t res_y_;
    Color *tex;
};
