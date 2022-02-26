#pragma once

#include <iostream>

#include "light.hh"
#include "vector3.hh"

class Point_Light : public Light
{
public:
    Point_Light(float intensity, Vector3 pos)
        : pos_(pos)
        , intensity_(intensity)
    {}

    float get_intensity()
    {
        return intensity_;
    }

    Vector3 get_pos()
    {
        return pos_;
    }

private:
    Vector3 pos_;
    float intensity_;
};
