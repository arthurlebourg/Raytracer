#include "blob.hh"
#include "fastnoise.hh"
#include "uniform_texture.hh"

class Planet : public Blob
{
public:
    Planet(Vector3 lower_left_corner, double side_length, int step,
           double threshold, int seed)
        : Blob(lower_left_corner, side_length, step, threshold)
        , seed_(seed)
        , side_length_(side_length)
    {
        center_ = Vector3(corner_.x() + side_length_ / 2,
                          corner_.y() + side_length_ / 2,
                          corner_.z() + side_length_ / 2);
    }

    std::vector<std::shared_ptr<Triangle>>
    render(std::shared_ptr<Texture_Material> texture);

private:
    std::vector<Triangle>
    get_sub_triangles(Sub_Cube sub_cube, int index,
                      std::shared_ptr<Texture_Material> texture);

    double evaluate_potential(Vector3 point);
    double sum_octave(double init, size_t num_iterations, double x, double y,
                      double z, double persistence, double scale, double low,

                      double high);
    int seed_;
    FastNoiseLite noise_;
    double side_length_;
    Vector3 center_;
};
