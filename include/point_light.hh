#pragma once

#include <iostream>

class Point_Light : Light
{
public:
    Point_Light(float intensity, Vector3 pos_)
        : pos_(pos)
        , intensity_(intensity)
    {}

    float get_intensity()
    {
        return intensity_;
    }
};
