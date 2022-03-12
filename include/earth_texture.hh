#pragma once

#include <cmath>
#include <iostream>

#include "fastnoise.hh"
#include "image.hh"
#include "texture_material.hh"
#include "utils.hh"

class Earth_Texture : public Texture_Material
{
public:
    Earth_Texture(int seed, size_t res_x, size_t res_y)
        : seed_(seed)
        , res_x_(res_x)
        , res_y_(res_y)
    {
        noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        noise_.SetSeed(seed);
        tex = (Color *)calloc(sizeof(Color), res_x * res_y);
        Image texture = Image("tex_earth.ppm", res_x, res_y);
        for (size_t y = 0; y < res_y_; y++)
        {
            for (size_t x = 0; x < res_x_; x++)
            {
                Color c(25, 77, 150);
                double r = sum_octave(8, x, y, 0.5, 0.5, 0, 1);
                if (r > 0.75)
                    c = Color(187, 170, 128);
                if (r > 0.55)
                    c = Color(64, 96, 40);
                tex[y * res_x_ + x] = c;
                texture.set(c, x, y);
            }
        }

        texture.save();
    }

    Material get_Material(Vector3 point, Vector3 center)
    {
        Vector3 normal = (center - point).normalized();

        double tmp = atan2(normal.z(), normal.x());

        double test = asin(normal.y());

        double u = 0.5 + tmp / (2 * pi);
        double v = 0.5 + test / pi;

        int width = u * res_x_;
        int height = v * res_y_;

        size_t px = height + (width * res_x_);

        if (px > res_x_ * res_y_)
            std::cout << "bite" << std::endl;

        // return Material(Color(u*255, v*255, 0), 1,1);
        return Material(tex[px], 1, 1);
    }

private:
    int seed_;
    FastNoiseLite noise_;
    const size_t res_x_;
    const size_t res_y_;
    Color *tex;
    double sum_octave(size_t num_iterations, double x, double y,
                      double persistence, double scale, double low, double high)
    {
        double maxAmp = 0;
        double amp = 1;
        double freq = scale;
        double noise = 0;

        for (size_t i = 0; i < num_iterations; i++)
        {
            noise += noise_.GetNoise(x * freq, y * freq) * amp;
            maxAmp += amp;
            amp *= persistence;
            freq *= 2;
        }

        noise /= maxAmp;

        noise = noise * (high - low) / 2 + (high + low) / 2;

        return noise;
    }
};
