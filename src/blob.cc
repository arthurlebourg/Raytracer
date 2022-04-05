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

Vector3 Blob::get_vertex_pos(Vector3 pt1, Vector3 pt2, double v1, double v2)
{
    double t = (threshold_ - v1) / (v2 - v1);
    return pt1 + t * (pt2 - pt1);
}

Vector3 Blob::edge_to_vect(int edge, Sub_Cube sub_cube)
{
    Vector3 intermediate;
    Vector3 pt1;
    Vector3 pt2;

    double v1;
    double v2;

    int i1;
    int i2;
    switch (edge)
    {
    case 0:
        i1 = 0;
        i2 = 1;
        pt1 = sub_cube.potentials[i1];
        pt2 = sub_cube.potentials[i2];
        v1 = sub_cube.potentials_values[i1];
        v2 = sub_cube.potentials_values[i2];
        intermediate = get_vertex_pos(pt1, pt2, v1, v2);
        return Vector3(intermediate.x(), pt1.y(), pt1.z());
    case 1:
        i1 = 2;
        i2 = 1;
        pt1 = sub_cube.potentials[i1];
        pt2 = sub_cube.potentials[i2];
        v1 = sub_cube.potentials_values[i1];
        v2 = sub_cube.potentials_values[i2];
        intermediate = get_vertex_pos(pt1, pt2, v1, v2);
        return Vector3(pt1.x(), pt1.y(), intermediate.z());
    case 2:
        i1 = 3;
        i2 = 2;
        pt1 = sub_cube.potentials[i1];
        pt2 = sub_cube.potentials[i2];
        v1 = sub_cube.potentials_values[i1];
        v2 = sub_cube.potentials_values[i2];
        intermediate = get_vertex_pos(pt1, pt2, v1, v2);
        return Vector3(intermediate.x(), pt1.y(), pt1.z());
    case 3:
        i1 = 3;
        i2 = 0;
        pt1 = sub_cube.potentials[i1];
        pt2 = sub_cube.potentials[i2];
        v1 = sub_cube.potentials_values[i1];
        v2 = sub_cube.potentials_values[i2];
        intermediate = get_vertex_pos(pt1, pt2, v1, v2);
        return Vector3(pt1.x(), pt1.y(), intermediate.z());
    case 4:
        i1 = 4;
        i2 = 5;
        pt1 = sub_cube.potentials[i1];
        pt2 = sub_cube.potentials[i2];
        v1 = sub_cube.potentials_values[i1];
        v2 = sub_cube.potentials_values[i2];
        intermediate = get_vertex_pos(pt1, pt2, v1, v2);
        return Vector3(intermediate.x(), pt1.y(), pt1.z());
    case 5:
        i1 = 6;
        i2 = 5;
        pt1 = sub_cube.potentials[i1];
        pt2 = sub_cube.potentials[i2];
        v1 = sub_cube.potentials_values[i1];
        v2 = sub_cube.potentials_values[i2];
        intermediate = get_vertex_pos(pt1, pt2, v1, v2);
        return Vector3(pt1.x(), pt1.y(), intermediate.z());
    case 6:
        i1 = 7;
        i2 = 6;
        pt1 = sub_cube.potentials[i1];
        pt2 = sub_cube.potentials[i2];
        v1 = sub_cube.potentials_values[i1];
        v2 = sub_cube.potentials_values[i2];
        intermediate = get_vertex_pos(pt1, pt2, v1, v2);
        return Vector3(intermediate.x(), pt1.y(), pt1.z());
    case 7:
        i1 = 7;
        i2 = 4;
        pt1 = sub_cube.potentials[i1];
        pt2 = sub_cube.potentials[i2];
        v1 = sub_cube.potentials_values[i1];
        v2 = sub_cube.potentials_values[i2];
        intermediate = get_vertex_pos(pt1, pt2, v1, v2);
        return Vector3(pt1.x(), pt1.y(), intermediate.z());
    case 8:
        i1 = 4;
        i2 = 0;
        pt1 = sub_cube.potentials[i1];
        pt2 = sub_cube.potentials[i2];
        v1 = sub_cube.potentials_values[i1];
        v2 = sub_cube.potentials_values[i2];
        intermediate = get_vertex_pos(pt1, pt2, v1, v2);
        return Vector3(pt1.x(), intermediate.y(), pt1.z());
    case 9:
        i1 = 5;
        i2 = 1;
        pt1 = sub_cube.potentials[i1];
        pt2 = sub_cube.potentials[i2];
        v1 = sub_cube.potentials_values[i1];
        v2 = sub_cube.potentials_values[i2];
        intermediate = get_vertex_pos(pt1, pt2, v1, v2);
        return Vector3(pt1.x(), intermediate.y(), pt1.z());
    case 10:
        i1 = 6;
        i2 = 2;
        pt1 = sub_cube.potentials[i1];
        pt2 = sub_cube.potentials[i2];
        v1 = sub_cube.potentials_values[i1];
        v2 = sub_cube.potentials_values[i2];
        intermediate = get_vertex_pos(pt1, pt2, v1, v2);
        return Vector3(pt1.x(), intermediate.y(), pt1.z());
    case 11:
        i1 = 7;
        i2 = 3;
        pt1 = sub_cube.potentials[i1];
        pt2 = sub_cube.potentials[i2];
        v1 = sub_cube.potentials_values[i1];
        v2 = sub_cube.potentials_values[i2];
        intermediate = get_vertex_pos(pt1, pt2, v1, v2);
        return Vector3(pt1.x(), intermediate.y(), pt1.z());
    // this should never be reached
    default:
        return Vector3(0, 0, 0);
    }
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
        auto vertex1 = edge_to_vect(edges[i], sub_cube);
        auto vertex2 = edge_to_vect(edges[i + 1], sub_cube);
        auto vertex3 = edge_to_vect(edges[i + 2], sub_cube);

        auto triangle = Smooth_Triangle(vertex1, vertex2, vertex3, nullptr);
        set_texture(triangle);
        triangles.push_back(triangle);
    }
    return triangles;
}

int Blob::get_table_index(Sub_Cube &sub_cube)
{
    int index = 0;
    for (size_t i = 0; i < sub_cube.potentials.size(); i++)
    {
        auto potential_value = evaluate_potential(sub_cube.potentials[i]);
        sub_cube.potentials_values.push_back(potential_value);
        if (potential_value > threshold_)
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
                Vector3 pos(corner_.x() + i * sub_length,
                            corner_.y() + j * sub_length,
                            corner_.z() + k * sub_length);
                auto val = evaluate_potential(pos);
                grid_.set(i, j, k, val);
            }
        }
    }

    for (auto i = corner_.x(); i < corner_.x() + side_length_; i += sub_length)
    {
        for (auto j = corner_.y(); j < corner_.y() + side_length_;
             j += sub_length)
        {
            for (auto k = corner_.z(); k < corner_.z() + side_length_;
                 k += sub_length)
            {
                Vector3 corner(i, j, k);
                Sub_Cube sub_cube(corner, sub_length);
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
