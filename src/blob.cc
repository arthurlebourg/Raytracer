#include "blob.hh"

Sub_Cube::Sub_Cube(Vector3 corner, double side_length)
{
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

Blob::Blob(Vector3 lower_left_corner, double side_length, int step,
           double threshold)
    : corner_(lower_left_corner)
    , nb_step_(step)
    , side_length_(side_length)
    , threshold_(threshold)
{
    auto center = lower_left_corner + (side_length / 2);
    auto radius = distance(lower_left_corner, center);
    Sphere englobing_volume_(center, radius, nullptr);
}

Vector3 Blob::get_vertex_pos(Vector3 pt1, Vector3 pt2)
{
    double v1 = evaluate_potential(pt1);
    double v2 = evaluate_potential(pt2);

    double t = (threshold_ - v1) / (v2 - v1);
    return pt1 + t * (pt2 - pt1);
}

Vector3 Blob::edge_to_vect(int edge, Sub_Cube sub_cube)
{
    Vector3 intermediate;
    Vector3 pt1;
    Vector3 pt2;
    switch (edge)
    {
    case 0:
        pt1 = sub_cube.potentials[0];
        pt2 = sub_cube.potentials[1];
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(intermediate.x(), pt1.y(), pt1.z());
    case 1:
        pt1 = sub_cube.potentials[2];
        pt2 = sub_cube.potentials[1];
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(pt1.x(), pt1.y(), intermediate.z());
    case 2:
        pt1 = sub_cube.potentials[3];
        pt2 = sub_cube.potentials[2];
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(intermediate.x(), pt1.y(), pt1.z());
    case 3:
        pt1 = sub_cube.potentials[3];
        pt2 = sub_cube.potentials[0];
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(pt1.x(), pt1.y(), intermediate.z());
    case 4:
        pt1 = sub_cube.potentials[4];
        pt2 = sub_cube.potentials[5];
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(intermediate.x(), pt1.y(), pt1.z());
    case 5:
        pt1 = sub_cube.potentials[6];
        pt2 = sub_cube.potentials[5];
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(pt1.x(), pt1.y(), intermediate.z());
    case 6:
        pt1 = sub_cube.potentials[7];
        pt2 = sub_cube.potentials[6];
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(intermediate.x(), pt1.y(), pt1.z());
    case 7:
        pt1 = sub_cube.potentials[7];
        pt2 = sub_cube.potentials[4];
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(pt1.x(), pt1.y(), intermediate.z());
    case 8:
        pt1 = sub_cube.potentials[4];
        pt2 = sub_cube.potentials[0];
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(pt1.x(), intermediate.y(), pt1.z());
    case 9:
        pt1 = sub_cube.potentials[5];
        pt2 = sub_cube.potentials[1];
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(pt1.x(), intermediate.y(), pt1.z());
    case 10:
        pt1 = sub_cube.potentials[6];
        pt2 = sub_cube.potentials[2];
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(pt1.x(), intermediate.y(), pt1.z());
    case 11:
        pt1 = sub_cube.potentials[7];
        pt2 = sub_cube.potentials[3];
        intermediate = get_vertex_pos(pt1, pt2);
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
std::vector<Triangle>
Blob::get_sub_triangles(Sub_Cube sub_cube, int index,
                        std::shared_ptr<Texture_Material> texture)
{
    std::vector<Triangle> triangles;
    auto edges = tri_table[index];
    for (int i = 0; edges[i] != -1; i += 3)
    {
        auto triangle = Triangle(edge_to_vect(edges[i], sub_cube),
                                 edge_to_vect(edges[i + 1], sub_cube),
                                 edge_to_vect(edges[i + 2], sub_cube), texture);
        triangles.push_back(triangle);
    }
    return triangles;
}

int Blob::get_table_index(Sub_Cube sub_cube)
{
    int index = 0;
    for (size_t i = 0; i < sub_cube.potentials.size(); i++)
    {
        if (evaluate_potential(sub_cube.potentials[i]) > threshold_)
            index |= (1 << i);
    }
    return index;
}

std::vector<std::shared_ptr<Triangle>>
Blob::render(std::shared_ptr<Texture_Material> texture)
{
    std::vector<std::shared_ptr<Triangle>> triangles;
    // length of sub cube's edge
    auto sub_length = side_length_ / nb_step_;

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
                auto sub_triangles =
                    get_sub_triangles(sub_cube, index, texture);
                // concatenates sub vector's content to the main one
                for (auto triangle : sub_triangles)
                {
                    triangles.push_back(std::make_shared<Triangle>(triangle));
                }
            }
        }
    }
    return triangles;
}
