#include "blob/blob.hh"
#include "noise/noise.hh"
#include "texture_material/uniform_texture.hh"

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
        radius_ = (side_length / 2);
        center_ = Vector3(corner_.x() + radius_, corner_.y() + radius_,
                          corner_.z() + radius_);
        radius_ *= 0.7;
        Uniform_Texture moon_texture =
            Uniform_Texture(Material(Color(230, 169, 88), 1, 1));

        texture_ = std::make_shared<Uniform_Texture>(moon_texture);
    }

private:
    double evaluate_potential(Vector3 point);
    FastNoiseLite noise_;
    double side_length_;
    Vector3 center_;
    double radius_;

    std::shared_ptr<Uniform_Texture> texture_;

    void set_texture(Smooth_Triangle &triangle);
};
