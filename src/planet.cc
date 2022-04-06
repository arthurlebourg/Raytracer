#include "planet.hh"

double Planet::sum_octave(double init, size_t num_iterations, double x,
                          double y, double z, double persistence, double scale,
                          double low, double high)
{
    double maxAmp = 0;
    double amp = 1;
    double freq = scale;
    double noise = init;

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

double Planet::evaluate_potential(Vector3 point)
{
    noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise_.SetSeed(seed_);
    double planet_radius = side_length_ / 2;

    double dist_from_center = (point - center_).length();
    double half_radius = planet_radius / 2;
    double maxD = Vector3(half_radius, half_radius, half_radius).length();

    double density = dist_from_center / (maxD + 1) - 0.5;

    double res =
        sum_octave(0, 16, point.x(), point.y(), point.z(), 0.25, 1, -0.5, 0.5);

    return density + res;
}

void Planet::set_texture(Smooth_Triangle &triangle)
{
    triangle.set_texture(grass_tex_);
    Vector3 dst_to_center = triangle.get_center() - center_;
    double slope = dot(dst_to_center, triangle.normal(triangle.get_center()));
    if (slope < 0)
        slope = -slope;

    if (slope
            / (dst_to_center.length()
               * triangle.normal(triangle.get_center()).length())
        < 0.5)
        triangle.set_texture(dirt_tex_);

    double ratio = dst_to_center.length() / radius_;
    if (ratio > 1)
        triangle.set_texture(snow_tex_);
    if (ratio < 0.8)
        triangle.set_texture(water_tex_);
}
