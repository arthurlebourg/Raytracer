#pragma once

#include <iostream>

#include "raytracer/light.hh"
#include "raytracer/vector3.hh"

class Point_Light : public Light
{
public:
    Point_Light(double intensity, Vector3 pos)
        : pos_(pos)
        , intensity_(intensity)
    {}

    Point_Light(const Point_Light &pt)
        : pos_(pt.pos_)
        , intensity_(pt.intensity_)
    {}

    double get_intensity()
    {
        return intensity_;
    }

    Vector3 get_pos()
    {
        return pos_;
    }

    void set_position(Vector3 vec)
    {
        pos_ = vec;
    }

    std::shared_ptr<Light> clone()
    {
        return std::make_shared<Point_Light>(Point_Light(*this));
    }

private:
    Vector3 pos_;
    double intensity_;
};
