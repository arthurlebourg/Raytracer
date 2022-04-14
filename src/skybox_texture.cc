#include "planet_texture/skybox_texture.hh"

Skybox_Texture::Skybox_Texture(int seed, int res_x, int res_y)
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
            double star = sum_octave(noise_, 0, 2, x, y, 0.1, 10, 0, 1);
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

            double milky_cloud = sum_octave(noise_, 0, 8, x, y, 0.5, 1, 0, 1);
            milky_cloud *= std::fmax(0.0, 1.0 - gradient);

            double milky_stars =
                sum_octave(noise_, 0, 1, x * 2, y / 2, 1, 50, 0, 1);

            milky_stars = milky_stars > 0.90 ? milky_stars : 0;

            milky_stars *= std::fmax(0.0, 1.0 - gradient / 20) * 255;
            if (milky_stars > 225)
            {
                tex[y * res_x_ + x] =
                    Color(milky_stars, milky_stars, milky_stars);
                texture.set(Color(milky_stars, milky_stars, milky_stars), x, y);
                continue;
            }

            Color default_color =
                Color(170 * milky_cloud, 145 * milky_cloud, 115 * milky_cloud);

            // simple nebulae
            double cloud = sum_octave(noise_, 0, 16, x, y, 0.5, 0.1, 0, 1);
            Color space_blue(3, 4, 94);
            Color space_pink(200, 116, 178);
            default_color = default_color
                + interpolate(space_pink, space_blue, cloud) * (cloud * cloud);

            tex[y * res_x_ + x] = default_color;

            texture.set(default_color, x, y);
        }
    }

    texture.save();
}

Material Skybox_Texture::get_Material(Vector3 point, Vector3 center,
                                      double light_specular_intensity)
{
    Vector3 normal = (center - point).normalized();

    size_t px = mercator(normal, res_x_, res_y_);

    if (px > res_x_ * res_y_)
    {
        std::cout << "texture out of bound earth" << std::endl;
        return Material(Color(255, 255, 0), 1, 1);
    }

    // return Material(Color(u*255, v*255, 0), 1,1);
    return Material(tex[px] * light_specular_intensity, 1, 1);
}
