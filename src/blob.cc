#include "blob.hh"

#include "tri_table.hh"

// Returns the appropriate vertex from the edge indexes in the sub cube
Vector3 edge_to_vect(int edge)
{
    switch (edge)
    {
    case 0:
        return Vector3(0.5, 1, 1);
    case 1:
        return Vector3(1, 1, 0.5);
    case 2:
        return Vector3(0.5, 1, 0);
    case 3:
        return Vector3(0, 1, 0.5);
    case 4:
        return Vector3(0.5, 0, 1);
    case 5:
        return Vector3(1, 0, 0.5);
    case 6:
        return Vector3(0.5, 0, 0);
    case 7:
        return Vector3(0, 0, 0.5);
    case 8:
        return Vector3(0, 0.5, 1);
    case 9:
        return Vector3(1, 0.5, 1);
    case 10:
        return Vector3(1, 0.5, 0);
    case 11:
        return Vector3(0, 0.5, 0);
    // this should never be reached
    default:
        return Vector3(0, 0, 0);
    }
}

/*
    returns the list of triangles from the potential points inside the sub cube
    ( corner + side length) represents the sub cube
    index is the place to look for in tri_table
*/
std::vector<Triangle>
get_sub_triangles(Vector3 corner, float side_length, int index,
                  std::shared_ptr<Texture_Material> texture)
{
    std::vector<Triangle> triangles;
    auto edges = tri_table[index];
    for (int i = 0; edges[i] != -1; i += 3)
    {
        auto triangle =
            Triangle(edge_to_vect(edges[i]), edge_to_vect(edges[i + 1]),
                     edge_to_vect(edges[i + 2]), texture);
        triangle.rescale(side_length);
        triangle.move(corner);
        triangles.push_back(triangle);
    }
    return triangles;
}

int Blob::get_table_index(Vector3 corner, float side_length) const
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
        if (potential_func_(potentials[i]) > threshold_)
            index |= (1 << i);
    }
    return index;
}

std::vector<std::shared_ptr<Triangle>>
Blob::render(std::shared_ptr<Texture_Material> texture) const
{
    std::vector<std::shared_ptr<Triangle>> triangles;
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
                    triangles.push_back(std::make_shared<Triangle>(triangle));
                }
            }
        }
    }
    return triangles;
}
