#include "planet.hh"

double Planet::sum_octave(double init, size_t num_iterations, double x,
                          double y, double z, double persistence, double scale,
                          double low, double high)
{
    double maxAmp = 0;
    double amp = 1;
    double freq = scale;
    double noise = init;

    for (size_t i = 0; i < num_iterations; i++)
    {
        noise += noise_.GetNoise(x * freq, y * freq, z * freq) * amp;
        maxAmp += amp;
        amp *= persistence;
        freq *= 2;
    }

    noise /= maxAmp;

    noise = noise * (high - low) / 2 + (high + low) / 2;

    return noise;
}

double Planet::evaluate_potential(Vector3 point)
{
    noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise_.SetSeed(seed_);
    double planet_radius = side_length_ / 2;

    double dist_from_center = (point - center_).length();
    double half_radius = planet_radius / 2;
    double maxD = Vector3(half_radius, half_radius, half_radius).length();

    double density = dist_from_center / (maxD + 1) - 0.5;

    double res =
        sum_octave(0, 16, point.x(), point.y(), point.z(), 1, 50, -0.5, 0.5);

    return density + res;
}

std::vector<Triangle>
Planet::get_sub_triangles(Sub_Cube sub_cube, int index,
                          std::shared_ptr<Texture_Material> texture)
{
    texture = texture;
    std::vector<Triangle> triangles;
    auto edges = tri_table[index];
    Uniform_Texture grass_tex =
        Uniform_Texture(Material(Color(0, 255, 0), 1, 1));
    grass_tex = grass_tex;
    Uniform_Texture dirt_tex =
        Uniform_Texture(Material(Color(155, 118, 83), 1, 1));
    for (int i = 0; edges[i] != -1; i += 3)
    {
        auto triangle = Triangle(edge_to_vect(edges[i], sub_cube),
                                 edge_to_vect(edges[i + 1], sub_cube),
                                 edge_to_vect(edges[i + 2], sub_cube),
                                 std::make_shared<Uniform_Texture>(dirt_tex));

        Vector3 sphere_normal = triangle.get_center() - center_;
        double slope =
            dot(sphere_normal, triangle.normal(triangle.get_center()));
        if (slope < 0)
            slope = -slope;

        if (slope
                / (sphere_normal.length()
                   * triangle.normal(triangle.get_center()).length())
            > 0.5)
            triangle.set_texture(std::make_shared<Uniform_Texture>(grass_tex));
        triangles.push_back(triangle);
    }
    return triangles;
}

std::vector<std::shared_ptr<Triangle>>
Planet::render(std::shared_ptr<Texture_Material> texture)
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
