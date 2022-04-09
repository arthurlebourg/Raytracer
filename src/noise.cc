#include "noise.hh"

double sum_octave(FastNoiseLite &noise, double init, size_t num_iterations,
                  double x, double y, double z, double persistence,
                  double scale, double low, double high)
{
    double maxAmp = 0;
    double amp = 1;
    double freq = scale;
    double res = init;

    for (size_t i = 0; i < num_iterations; i++)
    {
        res += noise.GetNoise(x * freq, y * freq, z * freq) * amp;
        maxAmp += amp;
        amp *= persistence;
        freq *= 2;
    }

    res /= maxAmp;

    res = res * (high - low) / 2 + (high + low) / 2;

    return res;
}

double sum_octave(FastNoiseLite &noise, double init, size_t num_iterations,
                  double x, double y, double persistence, double scale,
                  double low, double high)
{
    double maxAmp = 0;
    double amp = 1;
    double freq = scale;
    double res = init;

    for (size_t i = 0; i < num_iterations; i++)
    {
        res += noise.GetNoise(x * freq, y * freq) * amp;
        maxAmp += amp;
        amp *= persistence;
        freq *= 2;
    }

    res /= maxAmp;

    res = res * (high - low) / 2 + (high + low) / 2;

    return res;
}

double sum_octave_bis(FastNoiseLite &noise, size_t w, double x, double y,
                      double z, double ko, double fs, double v, double alpha,
                      std::function<double(double)> theta, double low,
                      double high)
{
    double res = 0;

    for (double k = 1; k <= w; k++)
    {
        double coef = ko + fs * k;
        double random = theta(noise.GetNoise(coef * x, coef * y, coef * z));
        res += theta(pow(coef, -v) * (coef * random));
    }

    res = pow(res, alpha);
    return res * (high - low) / 2 + (high + low) / 2;
}

double sum_octave_bis(FastNoiseLite &noise, size_t w, double x, double y,
                      double ko, double fs, double v, double alpha,
                      std::function<double(double)> theta, double low,
                      double high)
{
    double res = 0;

    for (double k = 1; k <= w; k++)
    {
        double coef = ko + fs * k;
        double random = theta(noise.GetNoise(coef * x, coef * y));
        res += theta(pow(coef, -v) * (coef * random));
    }

    res = pow(res, alpha);
    return res * (high - low) / 2 + (high + low) / 2;
}
