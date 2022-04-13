#include "asteroid.hh"

#include <stdlib.h>

double Asteroid::evaluate_potential(Vector3 point)
{
    double x = point.x() - center_.x();
    double y = point.y() - center_.y();
    double z = point.z() - center_.z();

    double den = x * x + y * y + z * z;

    auto dist_to_center = (radius_ * radius_) / den;

    auto noise = sum_octave(noise_, 0, 5, point.x(), point.y(), point.z(), 0.5,
                            1, -threshold_ / 2, threshold_ / 2);

    return dist_to_center + noise;
}

void Asteroid::set_texture(Smooth_Triangle &triangle)
{
    triangle.set_texture(texture_);
}
