#include "moon.hh"

#include <stdlib.h>

// inspired by Coding Adventure: Procedural Moons and Planets
// https://www.youtube.com/watch?v=lctXaT9pxA0&t=402s

double Moon::evaluate_potential(Vector3 point)
{
    double x = point.x() - center_.x();
    double y = point.y() - center_.y();
    double z = point.z() - center_.z();

    double den = x * x + y * y + z * z;

    auto dist_to_center = (radius_ * radius_) / den;
    if (dist_to_center > threshold_)
        return dist_to_center;

    auto noise = sum_octave(noise_, 0, 16, point.x(), point.y(), point.z(), 100,
                            100, -0.5, 0.5);

    return dist_to_center + abs(noise);

    /*
    for (auto crater : craters_)
    {
        auto crater_center = crater.get_center();
        auto crater_radius = crater.get_radius();

        x = point.x() - crater_center.x();
        y = point.y() - crater_center.y();
        z = point.z() - crater_center.z();

        den = x * x + y * y + z * z;

        auto crater_height = (crater_radius * crater_radius) / den;
        sphere_height -= crater_height;
    }
    */
}

void Moon::set_texture(Smooth_Triangle &triangle)
{
    triangle.set_texture(texture_);
}
