#include "blob/moon.hh"

#include <stdlib.h>

double Moon::evaluate_potential(Vector3 point)
{
    double x = point.x() - center_.x();
    double y = point.y() - center_.y();
    double z = point.z() - center_.z();

    double den = x * x + y * y + z * z;

    auto sphere_height = (radius_ * radius_) / den;

    auto noise = sum_octave(noise_, 0, 5, point.x(), point.y(), point.z(), 0.1,
                            1, 0, threshold_ / 2);

    return sphere_height + noise;
}

void Moon::set_texture(Smooth_Triangle &triangle)
{
    triangle.set_texture(texture_);
}
