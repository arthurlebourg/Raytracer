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

    Point_Light(const Point_Light &pt)
        : pos_(pt.pos_)
        , intensity_(pt.intensity_)
    {}

    float get_intensity()
    {
        return intensity_;
    }

    Vector3 get_pos()
    {
        return pos_;
    }

    std::shared_ptr<Light> clone()
    {
        return std::make_shared<Point_Light>(Point_Light(*this));
    }

private:
    Vector3 pos_;
    float intensity_;
};
