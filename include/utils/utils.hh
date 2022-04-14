#pragma once

#include <cmath>
#include <functional>
#include <map>

#include "texture_material/color.hh"

inline const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

inline Color interpolate(Color color1, Color color2, double fraction)
{
    return Color((color2.red() - color1.red()) * fraction + color1.red(),
                 (color2.green() - color1.green()) * fraction + color1.green(),
                 (color2.blue() - color1.blue()) * fraction + color1.blue());
}

inline size_t mercator(Vector3 normal, size_t res_x, size_t res_y)
{
    double y = normal.y() < -1.0 ? -1.0 : normal.y() > 1.0 ? 1.0 : normal.y();

    double tmp = atan2(normal.z(), normal.x());

    double test = asin(y);

    double u = 0.5 + tmp / (2 * pi);
    double v = 0.5 + test / pi;

    int width = u * (res_x - 1);
    int height = v * (res_y - 1);

    return height + (width * res_x);
}
