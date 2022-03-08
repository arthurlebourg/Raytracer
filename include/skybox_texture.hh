#pragma once

#include <cmath>
#include <iostream>

#include "texture_material.hh"

double Noise2(int x, int y, int z)
{
    int n = x + y * 57 + z * 17;
    n = (n << 13) ^ n;
    return (1.0
            - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff)
                / 1073741824.0);
}

class Skybox_Texture : public Texture_Material
{
public:
    Skybox_Texture()
    {}

    Material get_Material(Vector3 point)
    {
        double r = Noise2(static_cast<int>(round(point.x() / 5)),
                          static_cast<int>(round(point.y() / 5)),
                          static_cast<int>(round(point.z() / 5)));
        Color default_color = r > 0.98 ? Color(255, 255, 255) : Color(0, 0, 0);
        return Material(default_color, 1, 1);
        /*if (static_cast<int>(round(point.x()/100)) % 2 == 0 &&
        static_cast<int>(round(point.z()/100)) % 2 == 0) return
        Material(Color(255, 255, 255), 1, 1); return Material(Color(0, 0, 0), 1,
        1);
        */
    }

private:
};
