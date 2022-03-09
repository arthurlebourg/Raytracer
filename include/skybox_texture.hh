#pragma once

#include <cmath>
#include <functional>
#include <iostream>

#include "fastnoise.hh"
#include "texture_material.hh"

Color interpolate(Color color1, Color color2, double fraction)
{
    return Color((color2.red() - color1.red()) * fraction + color1.red(),
                 (color2.green() - color1.green()) * fraction + color1.green(),
                 (color2.blue() - color1.blue()) * fraction + color1.blue());
}

class Skybox_Texture : public Texture_Material
{
public:
    Skybox_Texture(int seed)
        : seed_(seed)
    {
        std::cout << "skybox seed: " << seed_ << std::endl;
        noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        noise_.SetSeed(seed);
    }

    Material get_Material(Vector3 point)
    {
        double star = sum_octave_bis(
            7, point.x(), point.y(), point.z(), 20, 3, -1, 8,
            [](double x) -> double { return x; }, 0, 255);
        /*double star = sum_octave_bis( // marrant
            7, point.x(), point.y(), point.z(), 2, 0.001, -1, 8,
            [](double x) -> double { return x; }, 0, 255);
        */

        Color stars(star, star, star);

        double cloud =
            sum_octave(16, point.x(), point.y(), point.z(), 0.5, 0.07, 0, 1);
        /*double cloud = sum_octave_bis(
            8, point.x(), point.y(), point.z(),
            0, 1, -1, 4,
            [](double x) -> double {return 1/x;},
            0, 255);
        return Material(Color(cloud, cloud, cloud), 1, 1);
        */
        Color space_blue(3, 4, 94);
        Color space_pink(200, 116, 178);
        Color default_color = stars
            + interpolate(space_pink, space_blue, cloud) * (cloud * cloud);
        return Material(default_color, 1, 1);
    }

private:
    int seed_;
    FastNoiseLite noise_;
    double sum_octave(size_t num_iterations, double x, double y, double z,
                      double persistence, double scale, double low, double high)
    {
        double maxAmp = 0;
        double amp = 1;
        double freq = scale;
        double noise = 0;

        for (size_t i = 0; i < num_iterations; i++)
        {
            noise += noise_.GetNoise(x * freq, y * freq, z * freq) * amp;
            maxAmp += amp;
            amp *= persistence;
            freq *= 2;
        }

        noise /= maxAmp;

        noise = noise * (high - low) / 2 + (high + low) / 2;

        return noise;
    }

    double sum_octave_bis(size_t w, double x, double y, double z, double ko,
                          double fs, double v, double alpha,
                          std::function<double(double)> theta, double low,
                          double high)
    {
        double noise = 0;

        for (double k = 1; k <= w; k++)
        {
            double coef = ko + fs * k;
            double random =
                theta(noise_.GetNoise(coef * x, coef * y, coef * z));
            noise += theta(pow(coef, -v) * (coef * random));
        }

        noise = pow(noise, alpha);
        return noise * (high - low) / 2 + (high + low) / 2;
    }
};
