#include "earth_texture.hh"

Earth_Texture::Earth_Texture(int seed, size_t res_x, size_t res_y)
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
            double terrain = sum_octave(noise_, 0, 16, x, y, 0.4, 0.3, 0, 1);
            double lights = sum_octave_bis(
                noise_, 7, x, y, 2, 1, -0.5, 8,
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
