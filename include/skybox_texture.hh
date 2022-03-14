#pragma once

#include <cmath>
#include <functional>
#include <iostream>

#include "fastnoise.hh"
#include "image.hh"
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

    Skybox_Texture(int seed, int res_x, int res_y)
        : seed_(seed)
        , res_x_(res_x)
        , res_y_(res_y)
    {
        noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        noise_.SetSeed(seed);
        tex = (Color *)calloc(sizeof(Color), res_x * res_y);
        Image texture = Image("tex_skybox.ppm", res_x_, res_y_);
        for (size_t y = 0; y < res_y_; y++)
        {
            for (size_t x = 0; x < res_x_; x++)
            {
                // stars
                double star = sum_octave(2, x, y, 0.1, 10, 0, 1);
                if (star > 0.95)
                {
                    tex[y * res_x_ + x] = Color(255, 255, 255);
                    texture.set(Color(255, 255, 255), x, y);
                    continue;
                }
                // milky way
                double island_size = (res_x + res_x) / 4;

                double distance_x = std::fabs(y - island_size) / 1.1;
                double distance_y = std::fabs(x - island_size * 1.25) * 3;
                double distance = std::sqrt(distance_x * distance_x
                                            + distance_y * distance_y); // mask

                double max_width = island_size * 0.5 - 10.0;
                double delta = distance / max_width;
                double gradient = delta * delta;

                double milky_cloud = sum_octave(8, x, y, 0.5, 1, 0, 1);
                milky_cloud *= std::fmax(0.0, 1.0 - gradient);

                double milky_stars = sum_octave(1, x * 2, y / 2, 1, 50, 0, 1);

                milky_stars = milky_stars > 0.90 ? milky_stars : 0;

                milky_stars *= std::fmax(0.0, 1.0 - gradient / 20) * 255;
                if (milky_stars > 225)
                {
                    tex[y * res_x_ + x] =
                        Color(milky_stars, milky_stars, milky_stars);
                    texture.set(Color(milky_stars, milky_stars, milky_stars), x,
                                y);
                    continue;
                }

                Color default_color = Color(
                    170 * milky_cloud, 145 * milky_cloud, 115 * milky_cloud);

                // simple nebulae
                double cloud = sum_octave(16, x, y, 0.5, 0.1, 0, 1);
                Color space_blue(3, 4, 94);
                Color space_pink(200, 116, 178);
                default_color = default_color
                    + interpolate(space_pink, space_blue, cloud)
                        * (cloud * cloud);

                tex[y * res_x_ + x] = default_color;

                texture.set(default_color, x, y);
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

/*double star = sum_octave_bis(
  7, point.x(), point.y(), point.z(), 20, 3, -1, 8,
  [](double x) -> double { return x; }, 0, 255);
  Color stars(star, star, star);
 */
/*double star = sum_octave_bis( // marrant
  7, point.x(), point.y(), point.z(), 2, 0.001, -1, 8,
  [](double x) -> double { return x; }, 0, 255);
 */

/*double cloud = sum_octave_bis(
  8, point.x(), point.y(), point.z(),
  0, 1, -1, 4,
  [](double x) -> double {return 1/x;},
  0, 255);
  return Material(Color(cloud, cloud, cloud), 1, 1);
 */
