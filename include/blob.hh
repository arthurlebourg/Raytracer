#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <optional>

#include "texture_material.hh"
#include "triangle.hh"

inline float default_metaballs_function(Vector3 point)
{
    int nb_balls = 2;
    // each ball has its info in the same index for each array
    float ball_radiuses[] = { 1.0, 1.0 };
    float ball_center_x[] = { 0.0, 1.5 };
    float ball_center_y[] = { 0.0, 1.5 };
    float ball_center_z[] = { 3.0, 4.5 };

    float res = 0;
    for (int i = 0; i < nb_balls; i++)
    {
        float num = ball_radiuses[i] * ball_radiuses[i];
        float x = point.x() - ball_center_x[i];
        float y = point.y() - ball_center_y[i];
        float z = point.z() - ball_center_z[i];

        float den = x * x + y * y + z * z;

        // when we are in the ball's center we are above the threshold
        if (den == 0)
            return 100;

        res += num / den;
    }
    return res;
}

class Blob
{
public:
    Blob(Vector3 lower_left_corner, float side_length, int step,
         float threshold,
         std::function<float(Vector3)> func = default_metaballs_function)
        : corner_(lower_left_corner)
        , nb_step_(step)
        , side_length_(side_length)
        , threshold_(threshold)
        , potential_func_(func)
    {}

    std::vector<std::shared_ptr<Triangle>>
    render(std::shared_ptr<Texture_Material> texture) const;

private:
    Vector3 corner_;
    int nb_step_;
    float side_length_;
    float threshold_;
    std::function<float(Vector3)> potential_func_;

    /*
       Returns the index to look up for in the tri_table
       by evaluating potentials in the sub cube(corner + side_length)
     */
    int get_table_index(Vector3 corner, float side_length) const;
};
