#pragma once

#include <functional>
#include <iostream>

#include "color.hh"
#include "fastnoise.hh"
#include "image.hh"

class Blue_noise
{
public:
    Blue_noise(size_t width, size_t height, int seed)
        : width_(width)
        , height_(height)
        , seed_(seed)
    {
        noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        noise_.SetSeed(seed);
        data_ = static_cast<Color *>(calloc(width * height, sizeof(Color)));
        Image texture = Image("tex_bluenoise.ppm", width, height);
        for (size_t y = 0; y < height; y++)
        {
            for (size_t x = 0; x < width; x++)
            {
                double blue_noise = sum_octave_bis(
                    7, x * 3, y * 3, 2, 0.001, -1, 8,
                    [](double x) -> double { return x; }, 0, 255);
                Color c = Color(blue_noise, blue_noise, blue_noise);
                data_[y * width + x] = c;
                texture.set(c, x, y);
            }
        }
        texture.save();
    }

    ~Blue_noise()
    {
        // free(data_);
    }

    size_t width()
    {
        return width_;
    }

    size_t height()
    {
        return height_;
    }

    Color get(size_t x, size_t y)
    {
        return data_[y * width_ + x];
    }

private:
    size_t width_;
    size_t height_;
    int seed_;
    Color *data_;
    FastNoiseLite noise_;
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
