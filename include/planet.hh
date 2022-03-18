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
        radius_ = side_length / 2;
        center_ = Vector3(corner_.x() + radius_, corner_.y() + radius_,
                          corner_.z() + radius_);
        Uniform_Texture grass_tex =
            Uniform_Texture(Material(Color(86, 125, 70), 1, 0));
        Uniform_Texture dirt_tex =
            Uniform_Texture(Material(Color(155, 118, 83), 1, 0));
        Uniform_Texture snow_tex =
            Uniform_Texture(Material(Color(179, 218, 241), 1, 0));
        Uniform_Texture water_tex =
            Uniform_Texture(Material(Color(35, 137, 218), 1, 0));

        grass_tex_ = std::make_shared<Uniform_Texture>(grass_tex);
        dirt_tex_ = std::make_shared<Uniform_Texture>(dirt_tex);
        snow_tex_ = std::make_shared<Uniform_Texture>(snow_tex);
        water_tex_ = std::make_shared<Uniform_Texture>(water_tex);
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
    double radius_;

    std::shared_ptr<Uniform_Texture> grass_tex_;
    std::shared_ptr<Uniform_Texture> dirt_tex_;
    std::shared_ptr<Uniform_Texture> snow_tex_;
    std::shared_ptr<Uniform_Texture> water_tex_;
};
