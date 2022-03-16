#include "blob.hh"
#include "fastnoise.hh"

class Planet : public Blob
{
public:
    Planet(Vector3 lower_left_corner, double side_length, int step,
           double threshold, int seed)
        : Blob(lower_left_corner, side_length, step, threshold)
        , seed_(seed)
        , side_length_(side_length)
    {}

private:
    double evaluate_potential(Vector3 point);
    double sum_octave(double init, size_t num_iterations, double x, double y,
                      double z, double persistence, double scale, double low,
                      double high);
    int seed_;
    FastNoiseLite noise_;
    double side_length_;
};
