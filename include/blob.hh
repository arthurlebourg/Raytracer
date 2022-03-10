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
        get the postition on which the triangle vertex will be placed between
        two sub_cube vertex using linear interpolation
    */
    Vector3 get_vertex_pos(Vector3 pt1, Vector3 pt2);

    // Returns the appropriate vertex from the edge indexes in the sub cube
    // corner + side_length represents the sub cube
    Vector3 edge_to_vect(int edge, Vector3 corner, float side_length);

    /*
       returns the list of triangles from the potential points inside the sub
       cube (corner + side length) represents the sub cube index is the place to
       look for in tri_table
     */
    std::vector<Triangle>
    get_sub_triangles(Vector3 corner, float side_length, int index,
                      std::shared_ptr<Texture_Material> texture);

    /*
       Returns the index to look up for in the tri_table
       by evaluating potentials in the sub cube(corner + side_length)
     */
    int get_table_index(Vector3 corner, float side_length);
};
