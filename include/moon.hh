#include "blob.hh"
#include "noise.hh"
#include "sphere.hh"
#include "uniform_texture.hh"

class Moon : public Blob
{
public:
    Moon(Vector3 lower_left_corner, double side_length, int step,
         double threshold, int seed)
        : Blob(lower_left_corner, side_length, step, threshold)
        , side_length_(side_length)
    {
        noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        noise_.SetSeed(seed);
        radius_ = (side_length / 2) * 0.9;
        center_ = Vector3(corner_.x() + radius_, corner_.y() + radius_,
                          corner_.z() + radius_);
        Uniform_Texture moon_texture =
            Uniform_Texture(Material(Color(230, 169, 88), 0.5, 0));

        texture_ = std::make_shared<Uniform_Texture>(moon_texture);
        Vector3 crater_center(corner_.x() + radius_, corner_.y() + radius_,
                              corner_.z() + radius_ / 2);
        craters_.emplace_back(crater_center, 20, nullptr);
    }

private:
    double evaluate_potential(Vector3 point);
    FastNoiseLite noise_;
    double side_length_;
    Vector3 center_;
    double radius_;
    std::vector<Sphere> craters_;

    std::shared_ptr<Uniform_Texture> texture_;

    void set_texture(Smooth_Triangle &triangle);

    double rimWidth_ = 0.7;
    double rimSteep_ = 0.3;
};
