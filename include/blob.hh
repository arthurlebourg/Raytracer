#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "smooth_triangle.hh"
#include "sphere.hh"
#include "texture_material.hh"
#include "tri_table.hh"

// sub cube to split the blob
class Sub_Cube
{
public:
    Sub_Cube(Vector3 corner, double side_length);
    std::vector<Vector3> potentials;
};

// abstract class for blob
class Blob
{
public:
    Blob(Vector3 lower_left_corner, double side_length, int step,
         double threshold);
    Sphere englobing_volume_;

protected:
    Vector3 corner_;
    int nb_step_;
    double side_length_;
    double threshold_;

    /*
       Returns the index to look up for in the tri_table
       by evaluating potentials in the sub cube(corner + side_length)
     */
    int get_table_index(Sub_Cube sub_cube);

    /*
        get the postition on which the triangle vertex will be placed between
        two sub_cube vertex using linear interpolation
    */
    Vector3 get_vertex_pos(Vector3 pt1, Vector3 pt2);

    /*
       returns the list of triangles from the potential points inside the sub
       index is the place to look for in tri_table
     */
    std::vector<Triangle>
    get_sub_triangles(Sub_Cube sub_cube, int index,
                      std::shared_ptr<Texture_Material> texture);

    // Returns the appropriate vertex from the edge indexes in the sub cube
    Vector3 edge_to_vect(int edge, Sub_Cube sub_cube);

    // Render the desired forms with triangles
    std::vector<std::shared_ptr<Triangle>>
    render(std::shared_ptr<Texture_Material> texture);

private:
    virtual double evaluate_potential(Vector3 point) = 0;
};
