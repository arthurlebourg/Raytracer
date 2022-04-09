#include <functional>

#include "fastnoise.hh"
double sum_octave(FastNoiseLite &noise, double init, size_t num_iterations,
                  double x, double y, double z, double persistence,
                  double scale, double low, double high);

double sum_octave(FastNoiseLite &noise, double init, size_t num_iterations,
                  double x, double y, double persistence, double scale,
                  double low, double high);

double sum_octave_bis(FastNoiseLite &noise, size_t w, double x, double y,
                      double ko, double fs, double v, double alpha,
                      std::function<double(double)> theta, double low,
                      double high);

double sum_octave_bis(FastNoiseLite &noise, size_t w, double x, double y,
                      double z, double ko, double fs, double v, double alpha,
                      std::function<double(double)> theta, double low,
                      double high);
