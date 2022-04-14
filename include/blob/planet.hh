#include "blob/blob.hh"
#include "noise/noise.hh"
#include "texture_material/uniform_texture.hh"

class Planet : public Blob
{
public:
    Planet(Vector3 lower_left_corner, double side_length, int step,
           double threshold, int seed)
        : Blob(lower_left_corner, side_length, step, threshold)
        , side_length_(side_length)
    {
        noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        noise_.SetSeed(seed);
        radius_ = side_length / 2;
        center_ = Vector3(corner_.x() + radius_, corner_.y() + radius_,
                          corner_.z() + radius_);
        radius_ *= 0.8;
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

private:
    double evaluate_potential(Vector3 point);
    FastNoiseLite noise_;
    double side_length_;
    Vector3 center_;
    double radius_;

    std::shared_ptr<Uniform_Texture> grass_tex_;
    std::shared_ptr<Uniform_Texture> dirt_tex_;
    std::shared_ptr<Uniform_Texture> snow_tex_;
    std::shared_ptr<Uniform_Texture> water_tex_;

    void set_texture(Smooth_Triangle &triangle);
};