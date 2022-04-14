#include "planet.hh"

double Planet::evaluate_potential(Vector3 point)
{
    double dist_from_center = (point - center_).length();
    double half_radius = radius_ / 2;
    double maxD = Vector3(half_radius, half_radius, half_radius).length();

    double density = dist_from_center / (maxD + 1) - 0.5;

    double res = sum_octave(noise_, 0, 16, point.x(), point.y(), point.z(),
                            0.25, 1, -0.5, 0.5);

    return density + res;
}

void Planet::set_texture(Smooth_Triangle &triangle)
{
    triangle.set_texture(dirt_tex_);
    Vector3 dst_to_center = (triangle.get_center() - center_);

    double slope = dot(dst_to_center, triangle.get_regular_normal());
    double max_slope =
        dst_to_center.length() * triangle.get_regular_normal().length();
    if (slope < 0)
        slope = -slope;

    double slope_ratio = slope / max_slope;
    if (slope_ratio > 0.7)
        triangle.set_texture(grass_tex_);
    if (slope_ratio > 0.9)
        triangle.set_texture(water_tex_);

    double dist_ratio = dst_to_center.length() / (radius_);
    if (dist_ratio > 0.9 && slope_ratio > 0.7)
        triangle.set_texture(snow_tex_);
}
