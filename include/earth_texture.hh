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
                // double terrain = sum_octave(8, x, y, 0.5, 0.5, 0, 1);
                double terrain = sum_octave(16, x, y, 0.4, 0.3, 0, 1);
                // double lights = terrain > 0.54 ? sum_octave(2, y, x, 0.5,
                // 0.5, 0, 2) : 0;
                double lights = sum_octave_bis(
                    7, x, y, 2, 1, -0.5, 8,
                    [](double x) -> double { return x; }, 0, 255);
                lights *= 1 - terrain;
                // Color c = Color(terrain * 255, r >= 0.54 && r <= 0.55 ? 255 :
                // 0, 0);
                Color c = Color(
                    terrain * 255,
                    terrain * 255 > 150 && terrain * 255 < 175 ? lights : 0, 0);
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

        double y = normal.y() < -1.0 ? -1.0
            : normal.y() > 1.0       ? 1.0
                                     : normal.y();

        double tmp = atan2(normal.z(), normal.x());

        double test = asin(y);

        double u = 0.5 + tmp / (2 * pi);
        double v = 0.5 + test / pi;

        int width = u * (res_x_ - 1);
        int height = v * (res_y_ - 1);

        size_t px = height + (width * res_x_);

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

    double sum_octave_bis(size_t w, double x, double y, double ko, double fs,
                          double v, double alpha,
                          std::function<double(double)> theta, double low,
                          double high)
    {
        double noise = 0;

        for (double k = 1; k <= w; k++)
        {
            double coef = ko + fs * k;
            double random = theta(noise_.GetNoise(coef * x, coef * y));
            noise += theta(pow(coef, -v) * (coef * random));
        }

        noise = pow(noise, alpha);
        return noise * (high - low) / 2 + (high + low) / 2;
    }
};
