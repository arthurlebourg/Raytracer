#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <optional>

#include "texture_material.hh"
#include "triangle.hh"

class Blob
{
public:
    Blob(Vector3 lower_left_corner, float side_length, int step,
         float threshold, std::function<float(Vector3)> func)
        : corner_(lower_left_corner)
        , nb_step_(step)
        , side_length_(side_length)
        , threshold_(threshold)
        , potential_func_(func)
    {}

    std::vector<Triangle>
    Render(std::shared_ptr<Texture_Material> texture) const;

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
