#include "blob.hh"

#include "tri_table.hh"

Vector3 Blob::get_vertex_pos(Vector3 pt1, Vector3 pt2)
{
    float v1 = evaluate_potential(pt1);
    float v2 = evaluate_potential(pt2);

    float t = (threshold_ - v1) / (v2 - v1);
    return pt1 + t * (pt2 - pt1);
}

Vector3 Blob::edge_to_vect(int edge, Vector3 corner, float side_length)
{
    auto x = corner.x();
    auto y = corner.y();
    auto z = corner.z();

    Vector3 intermediate;
    Vector3 pt1;
    Vector3 pt2;
    switch (edge)
    {
    case 0:
        pt1 = Vector3(x, y + side_length, z + side_length);
        pt2 = Vector3(x + side_length, y + side_length, z + side_length);
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(intermediate.x(), pt1.y(), pt1.z());
    case 1:
        pt1 = Vector3(x + side_length, y + side_length, z);
        pt2 = Vector3(x + side_length, y + side_length, z + side_length);
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(pt1.x(), pt1.y(), intermediate.z());
    case 2:
        pt1 = Vector3(x, y + side_length, z);
        pt2 = Vector3(x + side_length, y + side_length, z);
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(intermediate.x(), pt1.y(), pt1.z());
    case 3:
        pt1 = Vector3(x, y + side_length, z);
        pt2 = Vector3(x, y + side_length, z + side_length);
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(pt1.x(), pt1.y(), intermediate.z());
    case 4:
        pt1 = Vector3(x, y, z + side_length);
        pt2 = Vector3(x + side_length, y, z + side_length);
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(intermediate.x(), pt1.y(), pt1.z());
    case 5:
        pt1 = Vector3(x + side_length, y, z);
        pt2 = Vector3(x + side_length, y, z + side_length);
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(pt1.x(), pt1.y(), intermediate.z());
    case 6:
        pt1 = Vector3(x, y, z);
        pt2 = Vector3(x + side_length, y, z);
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(intermediate.x(), pt1.y(), pt1.z());
    case 7:
        pt1 = Vector3(x, y, z);
        pt2 = Vector3(x, y, z + side_length);
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(pt1.x(), pt1.y(), intermediate.z());
    case 8:
        pt1 = Vector3(x, y, z + side_length);
        pt2 = Vector3(x, y + side_length, z + side_length);
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(pt1.x(), intermediate.y(), pt1.z());
    case 9:
        pt1 = Vector3(x + side_length, y, z + side_length);
        pt2 = Vector3(x + side_length, y + side_length, z + side_length);
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(pt1.x(), intermediate.y(), pt1.z());
    case 10:
        pt1 = Vector3(x + side_length, y, z);
        pt2 = Vector3(x + side_length, y + side_length, z);
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(pt1.x(), intermediate.y(), pt1.z());
    case 11:
        pt1 = Vector3(x, y, z);
        pt2 = Vector3(x, y + side_length, z);
        intermediate = get_vertex_pos(pt1, pt2);
        return Vector3(pt1.x(), intermediate.y(), pt1.z());
    // this should never be reached
    default:
        return Vector3(0, 0, 0);
    }
}

/*
    returns the list of triangles from the potential points inside the sub cube
    (corner + side length) represents the sub cube
    index is the place to look for in tri_table
*/
std::vector<Smooth_Triangle>
Blob::get_sub_triangles(Vector3 corner, float side_length, int index,
                        std::shared_ptr<Texture_Material> texture)
{
    std::vector<Smooth_Triangle> triangles;
    auto edges = tri_table[index];
    for (int i = 0; edges[i] != -1; i += 3)
    {
        auto triangle = Smooth_Triangle(
            edge_to_vect(edges[i], corner, side_length),
            edge_to_vect(edges[i + 1], corner, side_length),
            edge_to_vect(edges[i + 2], corner, side_length), texture);
        triangles.push_back(triangle);
    }
    return triangles;
}

int Blob::get_table_index(Vector3 corner, float side_length)
{
    std::vector<Vector3> potentials;
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

    int index = 0;
    for (size_t i = 0; i < potentials.size(); i++)
    {
        if (evaluate_potential(potentials[i]) > threshold_)
            index |= (1 << i);
    }
    return index;
}

std::vector<std::shared_ptr<Smooth_Triangle>>
Blob::render(std::shared_ptr<Texture_Material> texture)
{
    std::vector<std::shared_ptr<Smooth_Triangle>> triangles;
    // length of sub triangle's edge
    auto sub_length = side_length_ / nb_step_;

    for (float i = corner_.x(); i < nb_step_; i += sub_length)
    {
        for (float j = corner_.y(); j < nb_step_; j += sub_length)
        {
            for (float k = corner_.z(); k < nb_step_; k += sub_length)
            {
                Vector3 corner(i, j, k);
                auto index = get_table_index(corner, sub_length);
                auto sub_triangles =
                    get_sub_triangles(corner, sub_length, index, texture);
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
