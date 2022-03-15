#include "blob.hh"
#include "sphere.hh"

class Metaball : public Blob
{
public:
    Metaball(Vector3 lower_left_corner, float side_length, int step,
             float threshold)
        : Blob(lower_left_corner, side_length, step, threshold)
    {}

    std::vector<Sphere> balls;

private:
    float evaluate_potential(Vector3 point);
};
