#include "blob.hh"
#include "sphere.hh"

class Metaball : public Blob
{
public:
    Metaball(Vector3 lower_left_corner, double side_length, int step,
             double threshold, std::shared_ptr<Texture_Material> texture)
        : Blob(lower_left_corner, side_length, step, threshold)
        , texture_(texture)
    {}

    std::vector<Sphere> balls;

private:
    double evaluate_potential(Vector3 point);
    void set_texture(Triangle &triangle)
    {
        triangle.set_texture(texture_);
    }

    std::shared_ptr<Texture_Material> texture_;
};
