#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "texture_material.hh"
#include "triangle.hh"

// abstract class for blob
class Blob
{
public:
    Blob(Vector3 lower_left_corner, float side_length, int step,
         float threshold)
        : corner_(lower_left_corner)
        , nb_step_(step)
        , side_length_(side_length)
        , threshold_(threshold)
    {}

    // Render the desired forms with triangles
    std::vector<std::shared_ptr<Triangle>>
    render(std::shared_ptr<Texture_Material> texture);

protected:
    Vector3 corner_;
    int nb_step_;
    float side_length_;
    float threshold_;

private:
    virtual float evaluate_potential(Vector3 point) = 0;

    /*
       Returns the index to look up for in the tri_table
       by evaluating potentials in the sub cube(corner + side_length)
     */
    int get_table_index(Vector3 corner, float side_length);
};
