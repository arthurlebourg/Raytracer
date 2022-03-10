#pragma once

#include <cmath>
#include <iostream>

#include "fastnoise.hh"
#include "texture_material.hh"

class Earth_Texture : public Texture_Material
{
public:
    Earth_Texture(int seed)
        : seed_(seed)
    {
        noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        noise_.SetSeed(seed);
    }

    Material get_Material(Vector3 point)
    {
        double r =
            sum_octave(8, point.x(), point.y(), point.z(), 0.5, 0.5, 0, 1);
        if (r > 0.75)
            return Material(Color(187, 170, 128), 1, 1);
        if (r > 0.55)
            return Material(Color(64, 96, 40), 1, 1);
        return Material(Color(25, 77, 150), 1, 1);
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
};
