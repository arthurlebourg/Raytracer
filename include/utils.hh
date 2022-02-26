#pragma once

#include <cmath>

const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

inline double abs(double x)
{
    if (x < 0)
        return -x;
    return x;
}
