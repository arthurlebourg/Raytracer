#include "blob.hh"

Sub_Cube::Sub_Cube(Vector3 corner, double side_length)
{
    side_length_ = side_length;

    auto x = corner.x();
    auto y = corner.y();
    auto z = corner.z();

    // create all potential points in the cube
    potentials.emplace_back(x, y + side_length, z + side_length);
    potentials.emplace_back(x + side_length, y + side_length, z + side_length);
    potentials.emplace_back(x + side_length, y + side_length, z);
    potentials.emplace_back(x, y + side_length, z);
    potentials.emplace_back(x, y, z + side_length);
    potentials.emplace_back(x + side_length, y, z + side_length);
    potentials.emplace_back(x + side_length, y, z);
    potentials.emplace_back(x, y, z);
}

// source : https://web.cs.ucdavis.edu/~ma/ECS177/papers/marching_cubes.pdf
Vector3 get_gradient(Meshgrid &grid, size_t x, size_t y, size_t z)
{
    double grad_x = grid.at(x + 1, y, z) - grid.at(x - 1, y, z);
    double grad_y = grid.at(x, y + 1, z) - grid.at(x, y - 1, z);
    double grad_z = grid.at(x, y, z + 1) - grid.at(x, y, z - 1);
    return Vector3(grad_x, grad_y, grad_z);
}

void Sub_Cube::fill(Meshgrid &grid, size_t x, size_t y, size_t z)
{
    potentials_values.push_back(grid.at(x, y + 1, z + 1));
    gradients.push_back(get_gradient(grid, x, y + 1, z + 1));

    potentials_values.push_back(grid.at(x + 1, y + 1, z + 1));
    gradients.push_back(get_gradient(grid, x + 1, y + 1, z + 1));

    potentials_values.push_back(grid.at(x + 1, y + 1, z));
    gradients.push_back(get_gradient(grid, x + 1, y + 1, z));

    potentials_values.push_back(grid.at(x, y + 1, z));
    gradients.push_back(get_gradient(grid, x, y + 1, z));

    potentials_values.push_back(grid.at(x, y, z + 1));
    gradients.push_back(get_gradient(grid, x, y, z + 1));

    potentials_values.push_back(grid.at(x + 1, y, z + 1));
    gradients.push_back(get_gradient(grid, x + 1, y, z + 1));

    potentials_values.push_back(grid.at(x + 1, y, z));
    gradients.push_back(get_gradient(grid, x + 1, y, z));

    potentials_values.push_back(grid.at(x, y, z));
    gradients.push_back(get_gradient(grid, x, y, z));
}

pos_gradient Blob::get_vertex_pos(Sub_Cube sub_cube, int i1, int i2)
{
    auto pt1 = sub_cube.potentials[i1];
    auto pt2 = sub_cube.potentials[i2];
    auto v1 = sub_cube.potentials_values[i1];
    auto v2 = sub_cube.potentials_values[i2];
    auto grad1 = sub_cube.gradients[i1];
    auto grad2 = sub_cube.gradients[i2];
    double t = (threshold_ - v1) / (v2 - v1);
    return pos_gradient{ pt1 + t * (pt2 - pt1), grad1 + t * (grad2 - grad1) };
}

pos_gradient Blob::edge_to_vect(int edge, Sub_Cube sub_cube)
{
    int i1 = 0;
    int i2 = 0;

    switch (edge)
    {
    case 0:
        i1 = 0;
        i2 = 1;
        break;
    case 1:
        i1 = 2;
        i2 = 1;
        break;
    case 2:
        i1 = 3;
        i2 = 2;
        break;
    case 3:
        i1 = 3;
        i2 = 0;
        break;
    case 4:
        i1 = 4;
        i2 = 5;
        break;
    case 5:
        i1 = 6;
        i2 = 5;
        break;
    case 6:
        i1 = 7;
        i2 = 6;
        break;
    case 7:
        i1 = 7;
        i2 = 4;
        break;
    case 8:
        i1 = 4;
        i2 = 0;
        break;
    case 9:
        i1 = 5;
        i2 = 1;
        break;
    case 10:
        i1 = 6;
        i2 = 2;
        break;
    case 11:
        i1 = 7;
        i2 = 3;
        break;
    }
    return get_vertex_pos(sub_cube, i1, i2);
}

/*
    returns the list of triangles from the potential points inside the sub cube
    index is the place to look for in tri_table
*/
std::vector<Smooth_Triangle> Blob::get_sub_triangles(Sub_Cube sub_cube,
                                                     int index)
{
    std::vector<Smooth_Triangle> triangles;
    auto edges = tri_table[index];
    for (int i = 0; edges[i] != -1; i += 3)
    {
        // vector + normal at each vertex
        auto p_g1 = edge_to_vect(edges[i], sub_cube);
        auto p_g2 = edge_to_vect(edges[i + 1], sub_cube);
        auto p_g3 = edge_to_vect(edges[i + 2], sub_cube);

        auto triangle = Smooth_Triangle(p_g1.position, p_g2.position,
                                        p_g3.position, p_g1.gradient,
                                        p_g2.gradient, p_g3.gradient, nullptr);
        set_texture(triangle);
        triangles.push_back(triangle);
    }
    return triangles;
}

int Blob::get_table_index(Sub_Cube &sub_cube)
{
    int index = 0;
    for (size_t i = 0; i < sub_cube.potentials_values.size(); i++)
    {
        if (sub_cube.potentials_values[i] > threshold_)
            index |= (1 << i);
    }
    return index;
}

std::vector<std::shared_ptr<Smooth_Triangle>> Blob::render()
{
    std::vector<std::shared_ptr<Smooth_Triangle>> triangles;
    // length of sub cube's edge
    auto sub_length = side_length_ / nb_step_;

    // fill the meshgrid
    for (auto i = 0; i < nb_step_ + 2; i += 1)
    {
        for (auto j = 0; j < nb_step_ + 2; j += 1)
        {
            for (auto k = 0; k < nb_step_ + 2; k += 1)
            {
                Vector3 pos(corner_.x() + (i - 1) * sub_length,
                            corner_.y() + (j - 1) * sub_length,
                            corner_.z() + (k - 1) * sub_length);
                auto val = evaluate_potential(pos);
                grid_.set(i, j, k, val);
            }
        }
    }

    // render the triangles
    for (auto i = 1; i <= nb_step_; i += 1)
    {
        for (auto j = 1; j <= nb_step_; j += 1)
        {
            for (auto k = 1; k <= nb_step_; k += 1)
            {
                Vector3 pos(corner_.x() + (i - 1) * sub_length,
                            corner_.y() + (j - 1) * sub_length,
                            corner_.z() + (k - 1) * sub_length);
                Sub_Cube sub_cube(pos, sub_length);
                sub_cube.fill(grid_, i, j, k);

                auto index = get_table_index(sub_cube);
                auto sub_triangles = get_sub_triangles(sub_cube, index);
                // concatenates sub vector's content to the main one
                for (auto triangle : sub_triangles)
                {
                    triangles.push_back(
                        std::make_shared<Smooth_Triangle>(triangle));
                }
            }
        }
    }

    return triangles;
}
