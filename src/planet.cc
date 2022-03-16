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

    Vector3 center =
        Vector3(corner_.x() + side_length_ / 2, corner_.y() + side_length_ / 2,
                corner_.z() + side_length_ / 2);

    double dist_from_center = (point - center).length();
    double half_radius = planet_radius / 2;
    double maxD = Vector3(half_radius, half_radius, half_radius).length();

    double density = dist_from_center / (maxD + 1) - 0.5;

    double res =
        sum_octave(0, 8, point.x(), point.y(), point.z(), 1, 0.5, -0.5, 0.5);

    return density + res;
}
