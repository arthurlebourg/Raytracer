#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "smooth_triangle.hh"
#include "texture_material.hh"
#include "tri_table.hh"

// 3d array with all potential values
class Meshgrid
{
public:
    Meshgrid()
        : buffer(nullptr)
    {}

    void init(size_t len)
    {
        buffer = new double[len * len * len];
        len_x = len;
        len_y = len;
        len_z = len;
    }

    ~Meshgrid()
    {
        delete[] buffer;
    }

    double at(size_t x, size_t y, size_t z)
    {
        return buffer[x * len_y * len_z + y * len_z + z];
    }

    void set(size_t x, size_t y, size_t z, double val)
    {
        buffer[x * len_y * len_z + y * len_z + z] = val;
    }

private:
    size_t len_x;
    size_t len_y;
    size_t len_z;
    double *buffer;
};

// sub cube to split the blob
class Sub_Cube
{
public:
    Sub_Cube(Vector3 corner, double side_length);
    // coordinates of cube vertexs
    std::vector<Vector3> potentials;
    // potential value at each vertex like in meshgrid
    std::vector<double> potentials_values;
    // normal at each vertex for smooth triangle creation
    std::vector<Vector3> gradient;
    double side_length_;

    // get the potential and gradient from the meshgrid
    // x, y, z is the coordinates of the lower left corner in the meshgrid
    void fill(Meshgrid &grid, size_t x, size_t y, size_t z);
};

// abstract class for blob
class Blob
{
public:
    Blob(Vector3 lower_left_corner, double side_length, int step,
         double threshold)
        : corner_(lower_left_corner)
        , nb_step_(step)
        , side_length_(side_length)
        , threshold_(threshold)
    {
        grid_.init(step + 2);
    }

    // Render the desired forms with triangles
    std::vector<std::shared_ptr<Smooth_Triangle>> render();

protected:
    Vector3 corner_;
    int nb_step_;
    double side_length_;
    double threshold_;
    // Returns the appropriate vertex from the edge indexes in the sub cube
    Vector3 edge_to_vect(int edge, Sub_Cube sub_cube);
    /*
       Returns the index to look up for in the tri_table
       by evaluating potentials in the sub cube(corner + side_length)
       also compute the potential at each cube vertex
     */
    int get_table_index(Sub_Cube &sub_cube);

private:
    Meshgrid grid_;
    virtual double evaluate_potential(Vector3 point) = 0;
    virtual void set_texture(Smooth_Triangle &triangle) = 0;

    /*
        get the postition on which the triangle vertex will be placed between
        two sub_cube vertex using linear interpolation
        pt1/2 = postion if sub sub vertex
        v1/2 = potential value
    */
    Vector3 get_vertex_pos(Vector3 pt1, Vector3 pt2, double v1, double v2);

    /*
       returns the list of triangles from the potential points inside the sub
       index is the place to look for in tri_table
     */
    std::vector<Smooth_Triangle> get_sub_triangles(Sub_Cube sub_cube,
                                                   int index);
};
