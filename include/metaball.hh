#include "blob.hh"
#include "sphere.hh"

class Metaball : public Blob
{
public:
    Metaball(Vector3 lower_left_corner, double side_length, int step,
             double threshold)
        : Blob(lower_left_corner, side_length, step, threshold)
    {}

    std::vector<Sphere> balls;

private:
    double evaluate_potential(Vector3 point);
};
